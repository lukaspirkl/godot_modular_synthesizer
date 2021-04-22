#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class ParameterNode : public SynthNode {
	GDCLASS(ParameterNode, SynthNode);

	void _name_changed(const String& text);
	void _trigger_down();
	void _trigger_up();

protected:
	static void _bind_methods();

public:
	ParameterNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
