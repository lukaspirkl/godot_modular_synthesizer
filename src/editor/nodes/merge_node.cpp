#include "scene/gui/label.h"

#include "number_value_editor.h"
#include "merge_node.h"

void MergeNode::_bind_methods()
{
}

void MergeNode::input_connected(int p_index)
{
	if (p_index == 1)
	{
		value->hide_value();
		_shrink_size();
	}
}

MergeNode::MergeNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Add");

	Label* a = memnew(Label);
	a->set_text("A");
	add_child(a);

	value = memnew(NumberValueEditor(this, p_synth, p_data, "B", "value", 0));
	add_child(value);

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
	set_slot(1, true, 10, Color(1, 1, 1), false, 10, Color(1, 1, 1));
}
