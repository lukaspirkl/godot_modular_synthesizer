#pragma once

#include "synth_node.h"

class ADSRNode : public SynthNode {
	GDCLASS(ADSRNode, SynthNode);

	void _a_changed(double value);
	void _d_changed(double value);
	void _s_changed(double value);
	void _r_changed(double value);

protected:
	static void _bind_methods();

public:
	ADSRNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};