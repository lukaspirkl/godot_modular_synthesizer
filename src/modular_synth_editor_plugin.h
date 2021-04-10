#pragma once

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/property_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/popup.h"
#include "scene/gui/tree.h"


class ModularSynthesizerEditor : public VBoxContainer {
	GDCLASS(ModularSynthesizerEditor, VBoxContainer);

	GraphEdit *graph;

public:
	ModularSynthesizerEditor();
};

class ModularSynthesizerEditorPlugin : public EditorPlugin {
	GDCLASS(ModularSynthesizerEditorPlugin, EditorPlugin);

	ModularSynthesizerEditor *modular_synthesizer_editor;
	EditorNode *editor;
	Button *button;

public:
	virtual String get_name() const { return "ModularSynthesizer"; }
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	ModularSynthesizerEditorPlugin(EditorNode *p_node);
	~ModularSynthesizerEditorPlugin();
};
