#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class ConstantGeneratorNode : public SynthNode {
	GDCLASS(ConstantGeneratorNode, SynthNode);

	void _value_changed(double value);

protected:
	static void _bind_methods();

public:
	ConstantGeneratorNode(Ref<NodeData> p_data);
};
