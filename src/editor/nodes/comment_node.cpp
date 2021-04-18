#include "scene/gui/label.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/line_edit.h"
#include "editor/editor_scale.h"

#include "comment_node.h"

CommentNode::CommentNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data)
	: SynthNode(p_synth, p_data)
{
	set_resizable(true);
	set_comment(true);

	content = memnew(Label);
	add_child(content);
	
	win = memnew(WindowDialog);
	win->set_title("Edit Comment");
	win->set_size(Size2(400, 300) * EDSCALE);
	win->set_resizable(true);
	add_child(win);

	VBoxContainer* vb = memnew(VBoxContainer);
	win->add_child(vb);
	vb->set_anchors_preset(Control::LayoutPreset::PRESET_WIDE);
	vb->set_margins_preset(Control::LayoutPreset::PRESET_WIDE, Control::LayoutPresetMode::PRESET_MODE_MINSIZE, 5);

	Label* win_label_title = memnew(Label);
	win_label_title->set_text("Title");
	vb->add_child(win_label_title);

	win_edit_title = memnew(LineEdit);
	win_edit_title->connect("text_changed", this, "_title_changed");
	vb->add_child(win_edit_title);

	Label* win_label_text = memnew(Label);
	win_label_text->set_text("Text");
	vb->add_child(win_label_text);

	win_edit_text = memnew(TextEdit);
	win_edit_text->set_wrap_enabled(true);
	win_edit_text->set_v_size_flags(SIZE_EXPAND_FILL);
	win_edit_text->connect("text_changed", this, "_text_changed");
	vb->add_child(win_edit_text);
	
	if (!data->get_params().has("size"))
	{
		data->get_params()["size"] = get_size() / EDSCALE;
	}
	else
	{
		Size2 size = data->get_params()["size"];
		set_size(size * EDSCALE);
	}

	if (!data->get_params().has("text"))
	{
		data->get_params()["text"] = "";
	}
	win_edit_text->set_text(data->get_params()["text"]);
	content->set_text(data->get_params()["text"]);

	if (!data->get_params().has("title"))
	{
		data->get_params()["title"] = "Comment";
	}
	win_edit_title->set_text(data->get_params()["title"]);
	set_title(data->get_params()["title"]);
}

void CommentNode::_resize_request(Vector2 p_new_minsize)
{
	set_size(p_new_minsize);
	data->get_params()["size"] = get_size() / EDSCALE;
}

void CommentNode::_gui_input_comment(const Ref<InputEvent>& p_event)
{
	Ref<InputEventMouseButton> mb = p_event;
	if (mb.is_valid() && mb->is_doubleclick() && mb->get_button_index() == BUTTON_LEFT) 
	{
		win->popup_centered();
		accept_event();
	}
}

void CommentNode::_text_changed()
{
	content->set_text(win_edit_text->get_text());
	data->get_params()["text"] = win_edit_text->get_text();
}

void CommentNode::_title_changed(const String& p_text)
{
	set_title(win_edit_title->get_text());
	data->get_params()["title"] = win_edit_title->get_text();
}

void CommentNode::_bind_methods()
{
	ClassDB::bind_method("_resize_request", &CommentNode::_resize_request);
	ClassDB::bind_method("_gui_input_comment", &CommentNode::_gui_input_comment);
	ClassDB::bind_method("_text_changed", &CommentNode::_text_changed);
	ClassDB::bind_method("_title_changed", &CommentNode::_title_changed);

}

void CommentNode::_notification(int p_what)
{
	if (p_what == NOTIFICATION_READY)
	{
		connect("resize_request", this, "_resize_request");
		connect("gui_input", this, "_gui_input_comment");
	}
}