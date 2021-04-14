#include "scene/gui/label.h"

#include "output_node.h"

OutputNode::OutputNode(Ref<NodeData> p_data)
	: SynthNode(p_data)
{
	set_title("Output");

	Label* label = memnew(Label);
	label->set_text("Output");
	add_child(label);
	set_slot(0, true, 10, Color(1, 1, 1), false, 10, Color(1, 1, 1));
}
