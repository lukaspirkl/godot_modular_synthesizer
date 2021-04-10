#include "modular_synthesizer_editor_plugin.h"
#include "editor/editor_scale.h"

void ModularSynthesizerEditorPlugin::edit(Object *p_object) {
	modular_synthesizer_editor->edit(Object::cast_to<ModularSynthesizer>(p_object));
}

bool ModularSynthesizerEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("ModularSynthesizer");
}

void ModularSynthesizerEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		button->show();
		editor->make_bottom_panel_item_visible(modular_synthesizer_editor);
		//modular_synthesizer_editor->update_custom_nodes();
		modular_synthesizer_editor->set_process_input(true);
		//modular_synthesizer_editor->set_process(true); //this is commented for shader editor
	} else {
		if (modular_synthesizer_editor->is_visible_in_tree())
			editor->hide_bottom_panel();
		button->hide();
		modular_synthesizer_editor->set_process_input(false);
		//modular_synthesizer_editor->set_process(false); //this is commented for shader editor
	}
}

ModularSynthesizerEditorPlugin::ModularSynthesizerEditorPlugin(EditorNode *p_node) {
	editor = p_node;
	modular_synthesizer_editor = memnew(ModularSynthesizerEditor);
	modular_synthesizer_editor->set_custom_minimum_size(Size2(0, 300) * EDSCALE);

	button = editor->add_bottom_panel_item("Modular Synthesizer", modular_synthesizer_editor);
	button->hide();
}

ModularSynthesizerEditorPlugin::~ModularSynthesizerEditorPlugin() {
}

void ModularSynthesizerEditor::edit(ModularSynthesizer *p_synth) {
	if (p_synth) {
		synth = Ref<ModularSynthesizer>(p_synth);
	} else {
		synth.unref();
	}

	if (synth.is_null()) {
		hide();
	}
}

ModularSynthesizerEditor::ModularSynthesizerEditor() {
	graph = memnew(GraphEdit);
	graph->get_zoom_hbox()->set_h_size_flags(SIZE_EXPAND_FILL);
	graph->set_v_size_flags(SIZE_EXPAND_FILL);
	graph->set_h_size_flags(SIZE_EXPAND_FILL);
	graph->set_drag_forwarding(this);
	add_child(graph);

	GraphNode *node = memnew(GraphNode);
	node->set_title("Output");
	node->set_offset(Vector2(100, 100));
	graph->add_child(node);
}
