#pragma once

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/property_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/popup.h"
#include "scene/gui/tree.h"

#include "modular_synthesizer.h"


class SynthNode : public GraphNode {
	GDCLASS(SynthNode, GraphNode);

	void _offset_changed();
	void _update_node_size_text(String p_text);

protected:
	Ref<NodeData> data;

	void _shrink_size();
	static void _bind_methods();
	void _notification(int p_what);

public:
	virtual void input_connected(int p_index) {};
	virtual void output_connected(int p_index) {};
	SynthNode(Ref<NodeData> p_data);
};



class ConstantGeneratorNode : public SynthNode {
	GDCLASS(ConstantGeneratorNode, SynthNode);

	void _value_changed(double value);

protected:
	static void _bind_methods();

public:
	ConstantGeneratorNode(Ref<NodeData> p_data);
};

class SineWaveGeneratorNode : public SynthNode {
	GDCLASS(SineWaveGeneratorNode, SynthNode);
	
	SpinBox* freq;

	void _freq_changed(double value);

protected:
	static void _bind_methods();

public:
	virtual void input_connected(int p_index);
	SineWaveGeneratorNode(Ref<NodeData> p_data);
};


class MergeNode : public SynthNode {
	GDCLASS(MergeNode, SynthNode);

	SpinBox* value;

	void _value_changed(double value);

protected:
	static void _bind_methods();

public:
	virtual void input_connected(int p_index);
	MergeNode(Ref<NodeData> p_data);
};

class MultiplyNode : public MergeNode {
	GDCLASS(MultiplyNode, MergeNode);

public:
	MultiplyNode(Ref<NodeData> p_data);
};

class AddNode : public MergeNode {
	GDCLASS(AddNode, MergeNode);

public:
	AddNode(Ref<NodeData> p_data);
};


class OutputNode : public SynthNode {
	GDCLASS(OutputNode, SynthNode);

public:
	OutputNode(Ref<NodeData> p_data);
};




class ModularSynthesizerEditor : public VBoxContainer {
	GDCLASS(ModularSynthesizerEditor, VBoxContainer);

	Ref<ModularSynthesizer> synth;
	GraphEdit* graph;
	PopupMenu* context_menu;
	Point2 new_node_pos;

	SynthNode* _create_node(Ref<NodeData> p_data);
	void _open_context_menu(const Vector2& p_position);
	void _add_node(int p_id);
	void _connection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index);
	void _disconnection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index);
	void _delete_nodes_request();
	void _connect_node(const String& p_from, int p_from_port, const String& p_to, int p_to_port);
	void _refresh_graph();

protected:
	static void _bind_methods();

public:
	void edit(ModularSynthesizer* p_synth);
	ModularSynthesizerEditor();
};