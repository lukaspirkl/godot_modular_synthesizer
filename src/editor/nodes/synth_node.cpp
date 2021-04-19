#include "editor/editor_scale.h"
#include "synth_node.h"

void SynthNode::_offset_changed()
{
	data->set_position(get_offset());
}

void SynthNode::_update_node_size_text(String p_text)
{
	_shrink_size();
}

void SynthNode::_shrink_size()
{
	set_size(Vector2(1, 1));
}

void SynthNode::_bind_methods()
{
	ClassDB::bind_method("_offset_changed", &SynthNode::_offset_changed);
	ClassDB::bind_method("_update_node_size_text", &SynthNode::_update_node_size_text);
}

void SynthNode::_notification(int p_what)
{
	if (p_what == NOTIFICATION_READY)
	{
		connect("offset_changed", this, "_offset_changed");
	}
}

SynthNode::SynthNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: synth(p_synth), data(p_data)
{
	set_offset(p_data->get_position() * EDSCALE);
}
