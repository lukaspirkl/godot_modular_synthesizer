#include "add_node.h"

AddNode::AddNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: MergeNode(p_synth, p_data)
{
	set_title("Add");
}
