#pragma once

#include "scene/gui/box_container.h"

#include "../../node_data.h"
#include "../../modular_synthesizer.h"
#include "synth_node.h"

class TextValueEditor : public HBoxContainer {
	GDCLASS(TextValueEditor, HBoxContainer)

	Ref<NodeData> data;
	Ref<ModularSynthesizer> synth;
	String param_name;
	class LineEdit* value_line_edit;

	void _text_changed(const String& value);

protected:
	static void _bind_methods();

public:
	void hide_value();
	TextValueEditor(SynthNode* parent, Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data, const String& p_label, const String& p_param_name, const String& p_default_text);
};
