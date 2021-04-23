#pragma once

#include "synth_node.h"

class MetronomeNode : public SynthNode {
	GDCLASS(MetronomeNode, SynthNode);

public:
	MetronomeNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};