#include "scene/gui/line_edit.h"
#include "parameter_node.h"

void ParameterNode::_name_changed(const String& text)
{
	data->get_params()["name"] = text;
	synth->emit_signal("changed");
}

void ParameterNode::_bind_methods()
{
	ClassDB::bind_method("_name_changed", &ParameterNode::_name_changed);
}

ParameterNode::ParameterNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_title("Parameter");

	LineEdit* name = memnew(LineEdit);
	name->set_text(data->get_params()["name"]);
	name->connect("text_changed", this, "_name_changed");
	add_child(name);
	set_slot(0, false, 10, Color(1, 1, 1), true, 10, Color(1, 1, 1));
}
