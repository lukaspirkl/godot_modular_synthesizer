#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"

#include "text_value_editor.h"

void TextValueEditor::_text_changed(const String& value)
{
	data->get_params()[param_name] = value;
	synth->emit_signal("changed");
}

void TextValueEditor::_bind_methods()
{
	ClassDB::bind_method("_text_changed", &TextValueEditor::_text_changed);
}

TextValueEditor::TextValueEditor(SynthNode* parent, Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data, const String& p_label, const String& p_param_name, const String& p_default_text)
	: synth(p_synth), data(p_data), param_name(p_param_name)
{
	Label* label = memnew(Label);
	label->set_text(p_label);
	add_child(label);

	value_line_edit = memnew(LineEdit);
	if (!p_data->get_params().has(p_param_name))
	{
		p_data->get_params()[p_param_name] = p_default_text;
	}
	value_line_edit->set_text(p_data->get_params()[p_param_name]);
	value_line_edit->connect("text_changed", this, "_text_changed");
	value_line_edit->set_expand_to_text_length(true);
	value_line_edit->connect("text_changed", parent, "_update_node_size_text");
	value_line_edit->set_h_size_flags(SIZE_EXPAND_FILL);
	add_child(value_line_edit);
}

void TextValueEditor::hide_value()
{
	value_line_edit->set_visible(false);
}

