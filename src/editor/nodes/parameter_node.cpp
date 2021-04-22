#include "scene/gui/box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/button.h"
#include "scene/gui/spin_box.h"

#include "text_value_editor.h"
#include "number_value_editor.h"
#include "parameter_node.h"

void ParameterNode::_value_changed(double value)
{
	synth->set_parameter(data->get_params()["name"], (float)value);
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
	ClassDB::bind_method("_trigger_down", &ParameterNode::_trigger_down);
	ClassDB::bind_method("_trigger_up", &ParameterNode::_trigger_up);
	ClassDB::bind_method("_value_changed", &ParameterNode::_value_changed);
}

ParameterNode::ParameterNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Runtime Parameter");

	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));

	TextValueEditor* name = memnew(TextValueEditor(this, p_synth, p_data, "Name", "name", ""));
	add_child(name);

	NumberValueEditor* value = memnew(NumberValueEditor(this, p_synth, p_data, "Value", "value", 0));
	value->do_not_emit_change_signal();
	value->get_spin_box()->connect("value_changed", this, "_value_changed");
	add_child(value);

	Button* button = memnew(Button);
	button->set_text("Trigger");
	button->connect("button_down", this, "_trigger_down");
	button->connect("button_up", this, "_trigger_up");
	add_child(button);
}
