#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class ParameterNode : public SynthNode {
	GDCLASS(ParameterNode, SynthNode);

	void _trigger_down();
	void _trigger_up();
	void _value_changed(double value);

protected:
	static void _bind_methods();

public:
	ParameterNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
