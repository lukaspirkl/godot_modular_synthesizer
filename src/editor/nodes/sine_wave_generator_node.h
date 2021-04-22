#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class SineWaveGeneratorNode : public SynthNode {
	GDCLASS(SineWaveGeneratorNode, SynthNode);

	class ValueEditor* freq;

protected:
	static void _bind_methods();

public:
	virtual void input_connected(int p_index);
	SineWaveGeneratorNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
