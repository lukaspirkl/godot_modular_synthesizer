#include "multiply_node.h"

MultiplyNode::MultiplyNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: MergeNode(p_synth, p_data)
{
	set_title("Multiply");
}
