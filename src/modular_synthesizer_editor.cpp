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
	context_menu->add_item("Sine Wave", NodeData::NodeType::NODE_SINE_WAVE);
	context_menu->add_item("Square Wave", 1);
	context_menu->set_item_disabled(3, true);
	context_menu->add_item("Triangle Wave", 1);
	context_menu->set_item_disabled(4, true);
	context_menu->add_separator();
	context_menu->add_item("Add", NodeData::NodeType::NODE_ADD);
	context_menu->add_item("Multiply", NodeData::NodeType::NODE_MULTIPLY);
	context_menu->connect("id_pressed", this, "_add_node");
	graph->add_child(context_menu);
}

SynthNode* ModularSynthesizerEditor::_create_node(Ref<NodeData> p_data)
{
	switch (p_data->get_type())
	{
	case NodeData::NodeType::NODE_CONSTANT:
		return memnew(ConstantGeneratorNode(p_data));
	case NodeData::NodeType::NODE_SINE_WAVE:
		return memnew(SineWaveGeneratorNode(p_data));
	case NodeData::NodeType::NODE_ADD:
		return memnew(AddNode(p_data));
	case NodeData::NodeType::NODE_MULTIPLY:
		return memnew(MultiplyNode(p_data));
	default:
		return NULL;
	}
}

void ModularSynthesizerEditor::edit(ModularSynthesizer* p_synth) {
	if (p_synth) {
		synth = Ref<ModularSynthesizer>(p_synth);
		_refresh_graph();
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
	SynthNode* node = _create_node(data);
	graph->add_child(node); // This will also generate and set unique name
	synth->get_nodes()[node->get_name()] = Variant(data);
}

void ModularSynthesizerEditor::_connection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index)
{
	for (int i = synth->get_connections().size() - 1; i >= 0; i--)
	{
		Ref<ConnectionData> c = synth->get_connections().get(i);
		
		// Allow only single connection to each input
		if (c->get_to() == p_to && c->get_to_index() == p_to_index)
		{
			synth->get_connections().remove(i);
		}

		// Allow only single connection to each output
		if (c->get_from() == p_from && c->get_from_index() == p_from_index)
		{
			synth->get_connections().remove(i);
		}
	}

	Ref<ConnectionData> c = memnew(ConnectionData(p_from, p_from_index, p_to, p_to_index));
	synth->get_connections().append(c);

	_refresh_graph();
}

void ModularSynthesizerEditor::_disconnection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index)
{
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

	_refresh_graph();
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
			
			synth->get_nodes().erase(gn->get_name());
		}
	}

	_refresh_graph();
}

void ModularSynthesizerEditor::_connect_node(const String& p_from, int p_from_port, const String& p_to, int p_to_port)
{
	Object::cast_to<SynthNode>(graph->get_node(p_from))->output_connected(p_from_port);
	Object::cast_to<SynthNode>(graph->get_node(p_to))->input_connected(p_to_port);
	graph->connect_node(p_from, p_from_port, p_to, p_to_port);
}

void ModularSynthesizerEditor::_refresh_graph()
{
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
	for (const Variant* key = nodes.next(); key != NULL; key = nodes.next(key))
	{
		SynthNode* node = _create_node(nodes[*key]);
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
		_connect_node(c->get_from(), c->get_from_index(), c->get_to(), c->get_to_index());
	}
}




void SynthNode::_offset_changed()
{
	data->set_position(get_offset());
}

void SynthNode::_update_node_size_text(String p_text)
{
	_shrink_size();
}

void SynthNode::_shrink_size()
{
	set_size(Vector2(1, 1));
}

void SynthNode::_bind_methods()
{
	ClassDB::bind_method("_offset_changed", &SynthNode::_offset_changed);
	ClassDB::bind_method("_update_node_size_text", &SynthNode::_update_node_size_text);
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
	spin_box->get_line_edit()->set_expand_to_text_length(true);
	spin_box->get_line_edit()->connect("text_changed", this, "_update_node_size_text");
	add_child(spin_box);
	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}





void SineWaveGeneratorNode::_freq_changed(double value)
{
	data->get_params()["freq"] = value;
}

void SineWaveGeneratorNode::_bind_methods()
{
	ClassDB::bind_method("_freq_changed", &SineWaveGeneratorNode::_freq_changed);
}

void SineWaveGeneratorNode::input_connected(int p_index)
{
	freq->set_visible(false);
	_shrink_size();
}

SineWaveGeneratorNode::SineWaveGeneratorNode(Ref<NodeData> p_data)
	: SynthNode(p_data)
{
	set_title("Sine Wave");
	HBoxContainer* hb = memnew(HBoxContainer);
	add_child(hb);
	
	Label* label = memnew(Label);
	label->set_text("Freq");
	hb->add_child(label);

	freq = memnew(SpinBox);
	freq->set_allow_greater(true);
	freq->set_allow_lesser(true);
	freq->set_step(0.001);
	freq->set_suffix("hz");
	if (!data->get_params().has("freq"))
	{
		data->get_params()["freq"] = (double)440.0;
	}
	freq->set_value(data->get_params()["freq"]);
	freq->connect("value_changed", this, "_freq_changed");
	freq->get_line_edit()->set_expand_to_text_length(true);
	freq->get_line_edit()->connect("text_changed", this, "_update_node_size_text");
	hb->add_child(freq);

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
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





void MergeNode::_value_changed(double value)
{
	data->get_params()["value"] = value;
}

void MergeNode::_bind_methods()
{
	ClassDB::bind_method("_value_changed", &MergeNode::_value_changed);
}

void MergeNode::input_connected(int p_index)
{
	if (p_index == 1)
	{
		value->set_visible(false);
		_shrink_size();
	}
}

MergeNode::MergeNode(Ref<NodeData> p_data)
	: SynthNode(p_data)
{
	set_title("Add");

	Label* a = memnew(Label);
	a->set_text("A");
	add_child(a);
	
	HBoxContainer* hb = memnew(HBoxContainer);
	add_child(hb);
	Label* label = memnew(Label);
	label->set_text("B");
	hb->add_child(label);
	value = memnew(SpinBox);
	value->set_allow_greater(true);
	value->set_allow_lesser(true);
	value->set_step(0.001);
	if (!data->get_params().has("value"))
	{
		data->get_params()["value"] = (double)0;
	}
	value->set_value(data->get_params()["value"]);
	value->connect("value_changed", this, "_value_changed");
	value->get_line_edit()->set_expand_to_text_length(true);
	value->get_line_edit()->connect("text_changed", this, "_update_node_size_text");
	hb->add_child(value);

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
	set_slot(1, true, 10, Color(1, 1, 1), false, 10, Color(1, 1, 1));
}


MultiplyNode::MultiplyNode(Ref<NodeData> p_data)
	: MergeNode(p_data)
{
	set_title("Multiply");
}

AddNode::AddNode(Ref<NodeData> p_data)
	: MergeNode(p_data)
{
	set_title("Add");
}
