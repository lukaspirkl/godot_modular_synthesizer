#pragma once

#include "scene/gui/box_container.h"

#include "../../node_data.h"
#include "../../modular_synthesizer.h"
#include "synth_node.h"

class NumberValueEditor : public HBoxContainer {
	GDCLASS(NumberValueEditor, HBoxContainer)

	bool emit_change_signal = true;
	Ref<NodeData> data;
	Ref<ModularSynthesizer> synth;
	String param_name;
	class SpinBox* value_spin_box;

	void _value_changed(double value);

protected:
	static void _bind_methods();

public:
	class SpinBox* get_spin_box();
	NumberValueEditor* set_suffix(const String& p_suffix);
	NumberValueEditor* do_not_emit_change_signal();
	void hide_value();
	NumberValueEditor(SynthNode* parent, Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data, const String& p_label, const String& p_param_name, double p_default_value);
};
