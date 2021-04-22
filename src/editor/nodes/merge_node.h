#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class MergeNode : public SynthNode {
	GDCLASS(MergeNode, SynthNode);

	class NumberValueEditor* value;

protected:
	static void _bind_methods();

public:
	virtual void input_connected(int p_index);
	MergeNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
