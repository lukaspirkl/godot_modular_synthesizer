#pragma once

#include "synth_node.h"

class ADSRNode : public SynthNode {
	GDCLASS(ADSRNode, SynthNode);

protected:
	static void _bind_methods();

public:
	ADSRNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};