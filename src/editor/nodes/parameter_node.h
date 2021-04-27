#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class ParameterNode : public SynthNode {
	GDCLASS(ParameterNode, SynthNode);

	NumberValueEditor* min;
	NumberValueEditor* max;
	NumberValueEditor* default_value;

	void _is_trigger_toggled(bool p_value);

protected:
	static void _bind_methods();

public:
	ParameterNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
