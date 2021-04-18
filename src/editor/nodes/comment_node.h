#pragma once

#include "synth_node.h"
#include "../../node_data.h"


class CommentNode : public SynthNode {
	GDCLASS(CommentNode, SynthNode);

	class WindowDialog* win;
	class Label* content;
	class TextEdit* win_edit_text;
	class LineEdit* win_edit_title;

	void _resize_request(Vector2 p_new_minsize);
	void _gui_input_comment(const Ref<InputEvent>& p_event);
	void _text_changed();
	void _title_changed(const String& p_text);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	CommentNode(Ref<ModularSynthesizer> p_synth, Ref<NodeData> p_data);
};
