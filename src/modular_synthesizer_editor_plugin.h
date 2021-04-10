#pragma once

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/property_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/popup.h"
#include "scene/gui/tree.h"

#include "modular_synthesizer.h"

class ModularSynthesizerEditor : public VBoxContainer {
	GDCLASS(ModularSynthesizerEditor, VBoxContainer);

	Ref<ModularSynthesizer> synth;
	GraphEdit *graph;

public:
	void edit(ModularSynthesizer *p_synth);
	ModularSynthesizerEditor();
};

class ModularSynthesizerEditorPlugin : public EditorPlugin {
	GDCLASS(ModularSynthesizerEditorPlugin, EditorPlugin);

	ModularSynthesizerEditor *modular_synthesizer_editor;
	EditorNode *editor;
	Button *button;

public:
	virtual String get_name() const { return "ModularSynthesizer"; }
	virtual void edit(Object *p_object) override;
	virtual bool handles(Object *p_object) const override;
	virtual void make_visible(bool p_visible) override;

	ModularSynthesizerEditorPlugin(EditorNode *p_node);
	~ModularSynthesizerEditorPlugin();
};
