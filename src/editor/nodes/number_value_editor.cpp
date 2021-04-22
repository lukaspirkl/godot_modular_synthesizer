#include "scene/gui/label.h"
#include "scene/gui/spin_box.h"

#include "number_value_editor.h"

void NumberValueEditor::_value_changed(double value)
{
	data->get_params()[param_name] = value;
	if (emit_change_signal)
	{
		synth->emit_signal("changed");
	}
}

void NumberValueEditor::_bind_methods()
{
	ClassDB::bind_method("_value_changed", &NumberValueEditor::_value_changed);
}

NumberValueEditor::NumberValueEditor(SynthNode* parent, Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data, const String& p_label, const String& p_param_name, double p_default_value)
	: synth(p_synth), data(p_data), param_name(p_param_name)
{
	Label* label = memnew(Label);
	label->set_text(p_label);
	add_child(label);

	value_spin_box = memnew(SpinBox);
	value_spin_box->set_allow_greater(true);
	value_spin_box->set_allow_lesser(true);
	value_spin_box->set_step(0.001);
	if (!p_data->get_params().has(p_param_name))
	{
		p_data->get_params()[p_param_name] = p_default_value;
	}
	value_spin_box->set_value(p_data->get_params()[p_param_name]);
	value_spin_box->connect("value_changed", this, "_value_changed");
	value_spin_box->get_line_edit()->set_expand_to_text_length(true);
	value_spin_box->get_line_edit()->connect("text_changed", parent, "_update_node_size_text");
	add_child(value_spin_box);
}

NumberValueEditor* NumberValueEditor::do_not_emit_change_signal()
{
	emit_change_signal = false;
	return this;
}

SpinBox* NumberValueEditor::get_spin_box()
{
	return value_spin_box;
}

NumberValueEditor* NumberValueEditor::set_suffix(const String& p_suffix) {
	value_spin_box->set_suffix(p_suffix);
	return this;
}

void NumberValueEditor::hide_value()
{
	value_spin_box->set_visible(false);
}

