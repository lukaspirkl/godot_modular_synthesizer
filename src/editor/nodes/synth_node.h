#pragma once

#include "scene/gui/graph_node.h"

#include "../../node_data.h"

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
