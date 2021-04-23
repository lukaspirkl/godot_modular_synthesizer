#include "metronome_node.h"
#include "number_value_editor.h"

MetronomeNode::MetronomeNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Metronome");

	add_child(memnew(NumberValueEditor(this, p_synth, p_data, "BPM", "bpm", 0)));

	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
