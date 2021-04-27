#pragma once

#include "scene/gui/box_container.h"

#include "../modular_synthesizer.h"
#include "../node_data.h"

class ModularSynthesizerGraphEditor : public VBoxContainer {
	GDCLASS(ModularSynthesizerGraphEditor, VBoxContainer);

	Ref<ModularSynthesizer> synth;
	class GraphEdit* graph;
	class PopupMenu* context_menu;
	Point2 new_node_pos;

	class SynthNode* _create_node(Ref<NodeData> p_data);
	void _open_context_menu(const Vector2& p_position);
	void _add_node(int p_id);
	void _connection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index);
	void _disconnection_request(const String& p_from, int p_from_index, const String& p_to, int p_to_index);
	void _delete_nodes_request();
	void _scroll_changed(const Vector2& p_scroll);
	void _refresh_graph();

protected:
	static void _bind_methods();

public:
	void edit(Ref<ModularSynthesizer> p_synth);
	ModularSynthesizerGraphEditor();
};
