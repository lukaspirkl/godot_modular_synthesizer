#include "scene/gui/spin_box.h"
#include "scene/gui/box_container.h"
#include "scene/gui/label.h"

#include "sine_wave_generator_node.h"

void SineWaveGeneratorNode::_freq_changed(double value)
{
	data->get_params()["freq"] = value;
	synth->emit_signal("changed");
}

void SineWaveGeneratorNode::_bind_methods()
{
	ClassDB::bind_method("_freq_changed", &SineWaveGeneratorNode::_freq_changed);
}

void SineWaveGeneratorNode::input_connected(int p_index)
{
	freq->set_visible(false);
	_shrink_size();
}

SineWaveGeneratorNode::SineWaveGeneratorNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Sine Wave");
	HBoxContainer* hb = memnew(HBoxContainer);
	add_child(hb);

	Label* label = memnew(Label);
	label->set_text("Freq");
	hb->add_child(label);

	freq = memnew(SpinBox);
	freq->set_allow_greater(true);
	freq->set_allow_lesser(true);
	freq->set_step(0.001);
	freq->set_suffix("hz");
	if (!data->get_params().has("freq"))
	{
		data->get_params()["freq"] = (double)440.0;
	}
	freq->set_value(data->get_params()["freq"]);
	freq->connect("value_changed", this, "_freq_changed");
	freq->get_line_edit()->set_expand_to_text_length(true);
	freq->get_line_edit()->connect("text_changed", this, "_update_node_size_text");
	hb->add_child(freq);

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
