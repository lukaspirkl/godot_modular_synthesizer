#include "scene/gui/spin_box.h"
#include "scene/gui/label.h"
#include "scene/gui/box_container.h"

#include "merge_node.h"

void MergeNode::_value_changed(double value)
{
	data->get_params()["value"] = value;
}

void MergeNode::_bind_methods()
{
	ClassDB::bind_method("_value_changed", &MergeNode::_value_changed);
}

void MergeNode::input_connected(int p_index)
{
	if (p_index == 1)
	{
		value->set_visible(false);
		_shrink_size();
	}
}

MergeNode::MergeNode(Ref<NodeData> p_data)
	: SynthNode(p_data)
{
	set_title("Add");

	Label* a = memnew(Label);
	a->set_text("A");
	add_child(a);

	HBoxContainer* hb = memnew(HBoxContainer);
	add_child(hb);
	Label* label = memnew(Label);
	label->set_text("B");
	hb->add_child(label);
	value = memnew(SpinBox);
	value->set_allow_greater(true);
	value->set_allow_lesser(true);
	value->set_step(0.001);
	if (!data->get_params().has("value"))
	{
		data->get_params()["value"] = (double)0;
	}
	value->set_value(data->get_params()["value"]);
	value->connect("value_changed", this, "_value_changed");
	value->get_line_edit()->set_expand_to_text_length(true);
	value->get_line_edit()->connect("text_changed", this, "_update_node_size_text");
	hb->add_child(value);

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
	set_slot(1, true, 10, Color(1, 1, 1), false, 10, Color(1, 1, 1));
}
