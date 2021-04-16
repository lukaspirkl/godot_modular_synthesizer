#pragma once

#include "merge_node.h"
#include "../../node_data.h"

class MultiplyNode : public MergeNode {
	GDCLASS(MultiplyNode, MergeNode);

public:
	MultiplyNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
