#include "scene/gui/spin_box.h"
#include "scene/gui/box_container.h"
#include "scene/gui/label.h"

#include "number_value_editor.h"
#include "sine_wave_generator_node.h"

void SineWaveGeneratorNode::_bind_methods()
{
}

void SineWaveGeneratorNode::input_connected(int p_index)
{
	freq->hide_value();
	_shrink_size();
}

SineWaveGeneratorNode::SineWaveGeneratorNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Sine Wave");
	
	freq = memnew(NumberValueEditor(this, p_synth, p_data, "Freq", "freq", 440));
	freq->set_suffix("Hz");
	add_child(freq);

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
