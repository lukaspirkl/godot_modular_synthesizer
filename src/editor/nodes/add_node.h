#pragma once

#include "merge_node.h"
#include "../../node_data.h"

class AddNode : public MergeNode {
	GDCLASS(AddNode, MergeNode);

public:
	AddNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
