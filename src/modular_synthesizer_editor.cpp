#include "modular_synthesizer_editor.h"
#include "editor/editor_scale.h"

void ModularSynthesizerEditor::_bind_methods()
{
	ClassDB::bind_method("_open_context_menu", &ModularSynthesizerEditor::_open_context_menu);
	ClassDB::bind_method("_add_node", &ModularSynthesizerEditor::_add_node);
	ClassDB::bind_method("_connection_request", &ModularSynthesizerEditor::_connection_request);
	ClassDB::bind_method("_disconnection_request", &ModularSynthesizerEditor::_disconnection_request);
	ClassDB::bind_method("_delete_nodes_request", &ModularSynthesizerEditor::_delete_nodes_request);
}

ModularSynthesizerEditor::ModularSynthesizerEditor() {
	graph = memnew(GraphEdit);
	graph->get_zoom_hbox()->set_h_size_flags(SIZE_EXPAND_FILL);
	graph->set_v_size_flags(SIZE_EXPAND_FILL);
	graph->set_h_size_flags(SIZE_EXPAND_FILL);
	graph->set_drag_forwarding(this);
	graph->set_right_disconnects(true);
	graph->connect("popup_request", this, "_open_context_menu");
	graph->connect("connection_request", this, "_connection_request");
	graph->connect("disconnection_request", this, "_disconnection_request");
	graph->connect("delete_nodes_request", this, "_delete_nodes_request");
	add_child(graph);

	context_menu = memnew(PopupMenu);
	context_menu->add_item("Constant", NodeData::NodeType::NODE_CONSTANT);
	context_menu->add_separator();
	context_menu->add_item("Sine Wave", 1);
	context_menu->set_item_disabled(2, true);
	context_menu->add_item("Square Wave", 2);
	context_menu->set_item_disabled(3, true);
	context_menu->add_item("Triangle Wave", 3);
	context_menu->set_item_disabled(4, true);
	context_menu->connect("id_pressed", this, "_add_node");
	graph->add_child(context_menu);
}

void ModularSynthesizerEditor::edit(ModularSynthesizer* p_synth) {
	if (p_synth) {
		synth = Ref<ModularSynthesizer>(p_synth);

		graph->clear_connections();

		for (int i = 0; i < graph->get_child_count(); i++) 
		{
			Node* node = graph->get_child(i);
			if (Object::cast_to<GraphNode>(node)) {
				graph->remove_child(node);
				memdelete(node);
				i--;
			}
		}

		Dictionary nodes = synth->get_nodes();
		for (const Variant *key = nodes.next(); key != NULL; key = nodes.next(key))
		{
			Ref<NodeData> data = nodes[*key];
			SynthNode* node = NULL;
			switch (data->get_type())
			{
			case NodeData::NodeType::NODE_CONSTANT:
				node = memnew(ConstantGeneratorNode(data));
				break;
			default:
				continue;
			}
			
			node->set_name(*key);
			graph->add_child(node);
		}

		if (synth->get_output() == NULL)
		{
			Ref<NodeData> output_data = memnew(NodeData);
			output_data->set_position((graph->get_scroll_ofs() + (graph->get_size() * 0.5)) / EDSCALE);
			synth->set_output(output_data);
		}
		OutputNode* output = memnew(OutputNode(synth->get_output()));
		graph->add_child(output);

		for (size_t i = 0; i < synth->get_connections().size(); i++)
		{
			Ref<ConnectionData> c = synth->get_connections().get(i);
			graph->connect_node(c->get_from(), c->get_from_index(), c->get_to(), c->get_to_index());
		}
	}
	else {
		synth.unref();
	}

	if (synth.is_null()) {
		hide();
	}
}

void ModularSynthesizerEditor::_open_context_menu(const Vector2& p_position)
{
	new_node_pos = graph->get_local_mouse_position();
	context_menu->set_position(p_position);
	context_menu->popup();
}

void ModularSynthesizerEditor::_add_node(int p_id)
{
	NodeData::NodeType type = (NodeData::NodeType)p_id;

	Point2 position = graph->get_scroll_ofs();
	position += new_node_pos;

	Ref<NodeData> data = memnew(NodeData);
	data->set_position(position);
	data->set_type(type);

	SynthNode* node = NULL;
	switch (type)
	{
	case NodeData::NODE_CONSTANT:
		node = memnew(ConstantGeneratorNode(data));
		break;
	default:
		return;
	}
	
	graph->add_child(node); // This will also generate and set unique name
	synth->get_nodes()[node->get_name()] = Variant(data);
}

void ModularSynthesizerEditor::_connection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index)
{
	graph->connect_node(p_from, p_from_index, p_to, p_to_index);
	Ref<ConnectionData> c = memnew(ConnectionData(p_from, p_from_index, p_to, p_to_index));
	synth->get_connections().append(c);
}

void ModularSynthesizerEditor::_disconnection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index)
{
	graph->disconnect_node(p_from, p_from_index, p_to, p_to_index);
	for (size_t i = 0; i < synth->get_connections().size(); i++)
	{
		Ref<ConnectionData> c = synth->get_connections().get(i);
		if (c->get_from() == p_from && c->get_from_index() == p_from_index &&
			c->get_to() == p_to && c->get_to_index() == p_to_index)
		{
			synth->get_connections().remove(i);
			break;
		}
	}
}

void ModularSynthesizerEditor::_delete_nodes_request()
{
	for (int i = 0; i < graph->get_child_count(); i++)
	{
		if (Object::cast_to<OutputNode>(graph->get_child(i)) != NULL)
		{
			// OutputNode cannot be deleted
			continue;
		}

		GraphNode* gn = Object::cast_to<GraphNode>(graph->get_child(i));
		if (gn && gn->is_selected()) 
		{
			for (int i = synth->get_connections().size() - 1; i >= 0; i--)
			{
				Ref<ConnectionData> c = synth->get_connections().get(i);
				if (c->get_from() == gn->get_name() || c->get_to() == gn->get_name())
				{
					synth->get_connections().remove(i);
				}
			}

			List<GraphEdit::Connection> connections;
			graph->get_connection_list(&connections);
			for (List<GraphEdit::Connection>::Element* E = connections.front(); E; E = E->next()) {
				const GraphEdit::Connection& con = E->get();
				if (con.from == gn->get_name() || con.to == gn->get_name())
				{
					graph->disconnect_node(con.from, con.from_port, con.to, con.to_port);
				}
			}
			
			synth->get_nodes().erase(gn->get_name());
			graph->remove_child(gn);
			memdelete(gn);
		}
	}
}




void SynthNode::_offset_changed()
{
	data->set_position(get_offset());
}

void SynthNode::_bind_methods()
{
	ClassDB::bind_method("_offset_changed", &SynthNode::_offset_changed);
}

void SynthNode::_notification(int p_what)
{
	if (p_what == NOTIFICATION_READY)
	{
		connect("offset_changed", this, "_offset_changed");
	}
}

SynthNode::SynthNode(Ref<NodeData> p_data)
	: data(p_data)
{
	set_offset(p_data->get_position());
}





void ConstantGeneratorNode::_value_changed(double value)
{
	data->get_params()["value"] = value;
}

void ConstantGeneratorNode::_bind_methods()
{
	ClassDB::bind_method("_value_changed", &ConstantGeneratorNode::_value_changed);
}

ConstantGeneratorNode::ConstantGeneratorNode(Ref<NodeData> p_data)
	: SynthNode(p_data)
{
	set_title("Constant");

	SpinBox* spin_box = memnew(SpinBox);
	spin_box->set_allow_greater(true);
	spin_box->set_allow_lesser(true);
	spin_box->set_value(data->get_params()["value"]);
	spin_box->connect("value_changed", this, "_value_changed");
	add_child(spin_box);
	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}




OutputNode::OutputNode(Ref<NodeData> p_data)
	: SynthNode(p_data)
{
	set_title("Output");
	
	Label* label = memnew(Label);
	label->set_text("Output");
	add_child(label);
	set_slot(0, true, 10, Color(1, 1, 1), false, 10, Color(1, 1, 1));
}
