#ifndef MODULAR_SYNTHESIZER_H
#define MODULAR_SYNTHESIZER_H

#include "editor/editor_node.h"
#include "editor/editor_plugin.h"
#include "editor/property_editor.h"
#include "scene/gui/button.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/popup.h"
#include "scene/gui/tree.h"

#include "core/ring_buffer.h"
#include "servers/audio/audio_stream.h"

#include "../thirdparty/tonic/src/Tonic.h"

class ModularSynthesizer : public Resource {
	GDCLASS(ModularSynthesizer, Resource);
};

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

class MyAudioStream : public AudioStream {
	GDCLASS(MyAudioStream, AudioStream);

protected:
	static void _bind_methods();

public:
	virtual Ref<AudioStreamPlayback> instance_playback();
	virtual String get_stream_name() const;

	virtual float get_length() const;
	MyAudioStream();
};


class MyAudioStreamPlayback : public AudioStreamPlayback {

	GDCLASS(MyAudioStreamPlayback, AudioStreamPlayback);
	friend class MyAudioStream;
	bool active;
	float mixed;
	MyAudioStream *generator;
	uint64_t pos;
	Tonic::Synth synth;

protected:
	static void _bind_methods();

public:
	virtual void start(float p_from_pos = 0.0);
	virtual void stop();
	virtual bool is_playing() const;

	virtual int get_loop_count() const; //times it looped

	virtual float get_playback_position() const;
	virtual void seek(float p_time);

	virtual void mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames);

	MyAudioStreamPlayback();
};

#endif // MODULAR_SYNTHESIZER_H
