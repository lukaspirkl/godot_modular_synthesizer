#pragma once

#include "synth_node.h"
#include "../../node_data.h"

class OutputNode : public SynthNode {
	GDCLASS(OutputNode, SynthNode);

public:
	OutputNode(Ref<NodeData> p_data);
};
