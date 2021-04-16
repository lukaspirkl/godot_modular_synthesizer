#include "scene/gui/spin_box.h"

#include "constant_generator_node.h"

void ConstantGeneratorNode::_value_changed(double value)
{
	data->get_params()["value"] = value;
	synth->emit_signal("changed");
}

void ConstantGeneratorNode::_bind_methods()
{
	ClassDB::bind_method("_value_changed", &ConstantGeneratorNode::_value_changed);
}

ConstantGeneratorNode::ConstantGeneratorNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Constant");

	SpinBox* spin_box = memnew(SpinBox);
	spin_box->set_allow_greater(true);
	spin_box->set_allow_lesser(true);
	spin_box->set_value(data->get_params()["value"]);
	spin_box->connect("value_changed", this, "_value_changed");
	spin_box->get_line_edit()->set_expand_to_text_length(true);
	spin_box->get_line_edit()->connect("text_changed", this, "_update_node_size_text");
	add_child(spin_box);
	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
