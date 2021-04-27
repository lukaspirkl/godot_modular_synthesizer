#include "scene/gui/box_container.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/button.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/check_button.h"

#include "text_value_editor.h"
#include "number_value_editor.h"
#include "parameter_node.h"

void ParameterNode::_is_trigger_toggled(bool p_value)
{
	data->get_params()["is_trigger"] = p_value;
	if (p_value)
	{
		if (min->is_inside_tree()) remove_child(min);
		if (max->is_inside_tree()) remove_child(max);
		if (default_value->is_inside_tree()) remove_child(default_value);
	}
	else
	{
		if (!min->is_inside_tree()) add_child(min);
		if (!max->is_inside_tree()) add_child(max);
		if (!default_value->is_inside_tree()) add_child(default_value);
	}
	set_size(Size2(1, 1));
}

void ParameterNode::_bind_methods()
{
	ClassDB::bind_method("_is_trigger_toggled", &ParameterNode::_is_trigger_toggled);
}

ParameterNode::ParameterNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Runtime Parameter");

	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));

	TextValueEditor* name = memnew(TextValueEditor(this, p_synth, p_data, "Name", "name", ""));
	add_child(name);

	if (!data->get_params().has("is_trigger"))
	{
		data->get_params()["is_trigger"] = false;
	}
	CheckButton* is_trigger = memnew(CheckButton);
	is_trigger->set_text("Is Trigger");
	is_trigger->set_pressed(data->get_params()["is_trigger"]);
	is_trigger->connect("toggled", this, "_is_trigger_toggled");
	add_child(is_trigger);

	min = memnew(NumberValueEditor(this, p_synth, p_data, "Min", "min", 0));
	max = memnew(NumberValueEditor(this, p_synth, p_data, "Max", "max", 1));
	default_value = memnew(NumberValueEditor(this, p_synth, p_data, "Default", "default", 0));
	
	_is_trigger_toggled(data->get_params()["is_trigger"]);
}
