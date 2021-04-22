#include "scene/gui/line_edit.h"
#include "scene/gui/button.h"

#include "parameter_node.h"

void ParameterNode::_name_changed(const String& text)
{
	data->get_params()["name"] = text;
	synth->emit_signal("changed");
}

void ParameterNode::_trigger_down()
{
	synth->set_parameter(data->get_params()["name"], 1);
}

void ParameterNode::_trigger_up()
{
	synth->set_parameter(data->get_params()["name"], 0);
}

void ParameterNode::_bind_methods()
{
	ClassDB::bind_method("_name_changed", &ParameterNode::_name_changed);
	ClassDB::bind_method("_trigger_down", &ParameterNode::_trigger_down);
	ClassDB::bind_method("_trigger_up", &ParameterNode::_trigger_up);
}

ParameterNode::ParameterNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Parameter");

	LineEdit* name = memnew(LineEdit);
	name->set_text(data->get_params()["name"]);
	name->connect("text_changed", this, "_name_changed");
	add_child(name);
	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));

	Button* button = memnew(Button);
	button->set_text("Trigger");
	button->connect("button_down", this, "_trigger_down");
	button->connect("button_up", this, "_trigger_up");
	add_child(button);
}
