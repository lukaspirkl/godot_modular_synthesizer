#include "editor/editor_scale.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/popup_menu.h"

#include "modular_synthesizer_editor.h"
#include "nodes/constant_generator_node.h"
#include "nodes/add_node.h"
#include "nodes/multiply_node.h"
#include "nodes/output_node.h"
#include "nodes/sine_wave_generator_node.h"
#include "nodes/comment_node.h"
#include "../connection_data.h"

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
	context_menu->add_separator();
	context_menu->add_item("Comment", NodeData::NodeType::NODE_COMMENT);
	context_menu->connect("id_pressed", this, "_add_node");
	graph->add_child(context_menu);
}

SynthNode* ModularSynthesizerEditor::_create_node(Ref<NodeData> p_data)
{
	switch (p_data->get_type())
	{
	case NodeData::NodeType::NODE_CONSTANT:
		return memnew(ConstantGeneratorNode(synth, p_data));
	case NodeData::NodeType::NODE_SINE_WAVE:
		return memnew(SineWaveGeneratorNode(synth, p_data));
	case NodeData::NodeType::NODE_ADD:
		return memnew(AddNode(synth, p_data));
	case NodeData::NodeType::NODE_MULTIPLY:
		return memnew(MultiplyNode(synth, p_data));
	case NodeData::NodeType::NODE_COMMENT:
		return memnew(CommentNode(synth, p_data));
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
	synth->emit_signal("changed");
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
	}

	Ref<ConnectionData> c = memnew(ConnectionData(p_from, p_from_index, p_to, p_to_index));
	synth->get_connections().append(c);

	_refresh_graph();
	synth->emit_signal("changed");
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
	synth->emit_signal("changed");
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
	synth->emit_signal("changed");
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
	OutputNode* output = memnew(OutputNode(synth, synth->get_output()));
	graph->add_child(output);

	for (size_t i = 0; i < synth->get_connections().size(); i++)
	{
		Ref<ConnectionData> c = synth->get_connections().get(i);
		Object::cast_to<SynthNode>(graph->get_node(c->get_from()))->output_connected(c->get_from_index());
		Object::cast_to<SynthNode>(graph->get_node(c->get_to()))->input_connected(c->get_to_index());
		graph->connect_node(c->get_from(), c->get_from_index(), c->get_to(), c->get_to_index());
	}
}
