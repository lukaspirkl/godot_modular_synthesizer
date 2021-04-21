#include "scene/gui/spin_box.h"
#include "scene/gui/label.h"
#include "scene/gui/box_container.h"
#include "adsr_node.h"

void ADSRNode::_a_changed(double value)
{
	data->get_params()["a"] = value;
	synth->emit_signal("changed");
}

void ADSRNode::_d_changed(double value)
{
	data->get_params()["d"] = value;
	synth->emit_signal("changed");
}

void ADSRNode::_s_changed(double value)
{
	data->get_params()["s"] = value;
	synth->emit_signal("changed");
}

void ADSRNode::_r_changed(double value)
{
	data->get_params()["r"] = value;
	synth->emit_signal("changed");
}

void ADSRNode::_bind_methods()
{
	ClassDB::bind_method("_a_changed", &ADSRNode::_a_changed);
	ClassDB::bind_method("_d_changed", &ADSRNode::_d_changed);
	ClassDB::bind_method("_s_changed", &ADSRNode::_s_changed);
	ClassDB::bind_method("_r_changed", &ADSRNode::_r_changed);
}

ADSRNode::ADSRNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("ADSR Envelope");

	Label* trig_label = memnew(Label);
	trig_label->set_text("Trigger");
	add_child(trig_label);

	if (!data->get_params().has("a"))
	{
		data->get_params()["a"] = (double)0.01;
	}
	HBoxContainer* a_hb = memnew(HBoxContainer);
	add_child(a_hb);
	SpinBox* a_value = memnew(SpinBox);
	a_value->set_allow_greater(true);
	a_value->set_allow_lesser(true);
	a_value->set_step(0.001);
	a_value->set_value(data->get_params()["a"]);
	a_value->connect("value_changed", this, "_a_changed");
	a_hb->add_child(a_value);
	Label* a_label = memnew(Label);
	a_label->set_text("Attack");
	a_hb->add_child(a_label);

	if (!data->get_params().has("d"))
	{
		data->get_params()["d"] = (double)1;
	}
	HBoxContainer* d_hb = memnew(HBoxContainer);
	add_child(d_hb);
	SpinBox* d_value = memnew(SpinBox);
	d_value->set_allow_greater(true);
	d_value->set_allow_lesser(true);
	d_value->set_step(0.001);
	d_value->set_value(data->get_params()["d"]);
	d_value->connect("value_changed", this, "_d_changed");
	d_hb->add_child(d_value);
	Label* d_label = memnew(Label);
	d_label->set_text("Decay");
	d_hb->add_child(d_label);

	if (!data->get_params().has("s"))
	{
		data->get_params()["s"] = (double)1;
	}
	HBoxContainer* s_hb = memnew(HBoxContainer);
	add_child(s_hb);
	SpinBox* s_value = memnew(SpinBox);
	s_value->set_allow_greater(true);
	s_value->set_allow_lesser(true);
	s_value->set_step(0.001);
	s_value->set_value(data->get_params()["s"]);
	s_value->connect("value_changed", this, "_s_changed");
	s_hb->add_child(s_value);
	Label* s_label = memnew(Label);
	s_label->set_text("Sustain");
	s_hb->add_child(s_label);

	if (!data->get_params().has("r"))
	{
		data->get_params()["r"] = (double)1;
	}
	HBoxContainer* r_hb = memnew(HBoxContainer);
	add_child(r_hb);
	SpinBox* r_value = memnew(SpinBox);
	r_value->set_allow_greater(true);
	r_value->set_allow_lesser(true);
	r_value->set_step(0.001);
	r_value->set_value(data->get_params()["r"]);
	r_value->connect("value_changed", this, "_r_changed");
	r_hb->add_child(r_value);
	Label* r_label = memnew(Label);
	r_label->set_text("Release");
	r_hb->add_child(r_label);

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
