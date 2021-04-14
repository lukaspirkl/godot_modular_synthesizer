#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class SineWaveGeneratorNode : public SynthNode {
	GDCLASS(SineWaveGeneratorNode, SynthNode);

	class SpinBox* freq;

	void _freq_changed(double value);

protected:
	static void _bind_methods();

public:
	virtual void input_connected(int p_index);
	SineWaveGeneratorNode(Ref<NodeData> p_data);
};
