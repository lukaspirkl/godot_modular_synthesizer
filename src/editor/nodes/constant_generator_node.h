#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class ConstantGeneratorNode : public SynthNode {
	GDCLASS(ConstantGeneratorNode, SynthNode);

protected:
	static void _bind_methods();

public:
	ConstantGeneratorNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
