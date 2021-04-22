#include "scene/gui/spin_box.h"
#include "scene/gui/label.h"
#include "scene/gui/box_container.h"

#include "number_value_editor.h"
#include "adsr_node.h"

void ADSRNode::_bind_methods()
{
}

ADSRNode::ADSRNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("ADSR Envelope");

	Label* trig_label = memnew(Label);
	trig_label->set_text("Trigger");
	add_child(trig_label);

	add_child(memnew(NumberValueEditor(this, p_synth, p_data, "Attack", "a", 0.01)));
	add_child(memnew(NumberValueEditor(this, p_synth, p_data, "Decay", "d", 0)));
	add_child(memnew(NumberValueEditor(this, p_synth, p_data, "Sustain", "s", 1)));
	add_child(memnew(NumberValueEditor(this, p_synth, p_data, "Release", "r", 0.5)));

	set_slot(0, true, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
