#pragma once

#include "scene/gui/box_container.h"

#include "../modular_synthesizer.h"

class ModularSynthesizerPlayerEditor : public VBoxContainer {
	GDCLASS(ModularSynthesizerPlayerEditor, VBoxContainer);

	class ModularSynthesizerPlayback* playback = NULL;
	class AudioStreamPlayer* audio_stream_player;
	class Button* play_stop;

	List<Node*> property_nodes;

	void _button_pressed();
	void _res_changed();
	void _refresh_realtime_parameters();
	void _toggled(bool p_value, const String& p_name);
	void _value_changed(float p_value, const String& p_name);

protected:
	static void _bind_methods();

public:
	void edit(Ref<ModularSynthesizer> p_synth);
	ModularSynthesizerPlayerEditor();
};
