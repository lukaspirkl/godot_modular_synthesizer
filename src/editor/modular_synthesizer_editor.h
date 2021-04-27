#pragma once

#include "scene/gui/box_container.h"

#include "../modular_synthesizer.h"

class ModularSynthesizerEditor : public HBoxContainer {
	GDCLASS(ModularSynthesizerEditor, HBoxContainer);

	class ModularSynthesizerGraphEditor* graph;
	class ModularSynthesizerPlayerEditor* player;

public:
	void edit(Ref<ModularSynthesizer> p_synth);
	ModularSynthesizerEditor();
};
