#include "number_value_editor.h"
#include "constant_generator_node.h"

void ConstantGeneratorNode::_bind_methods()
{
}

ConstantGeneratorNode::ConstantGeneratorNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Constant");

	NumberValueEditor* value = memnew(NumberValueEditor(this, p_synth, p_data, "Value", "value", 0));
	add_child(value);

	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
