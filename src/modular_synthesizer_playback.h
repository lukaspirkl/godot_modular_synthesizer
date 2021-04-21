#pragma once

#include "servers/audio/audio_stream.h"

#include "../thirdparty/tonic/src/Tonic.h"
#include "modular_synthesizer.h"

class ModularSynthesizerPlayback : public AudioStreamPlayback {
	GDCLASS(ModularSynthesizerPlayback, AudioStreamPlayback);

	bool active;
	bool is_res_up_to_date = false;
	ModularSynthesizer* res;
	uint64_t pos;
	Tonic::Synth synth;
	Map<String, Tonic::Generator*> generators;
	Map<String, Tonic::ControlGenerator*> control_generators;

	String _get_node_connected_to(const String& name, int index);
	Tonic::Generator* _get_generator(const String& name);
	Tonic::Generator* _create_generator(const String& name);
	Tonic::ControlGenerator* _get_control_generator(const String& name);
	Tonic::ControlGenerator* _create_control_generator(const String& name);
	void _refresh_synth();

protected:
	static void _bind_methods();

public:
	void set_resource(ModularSynthesizer* res);

	void resource_changed();
	void parameter_changed(const String& p_name, float p_value);

	virtual void start(float p_from_pos = 0.0);
	virtual void stop();
	virtual bool is_playing() const;

	virtual int get_loop_count() const; //times it looped

	virtual float get_playback_position() const;
	virtual void seek(float p_time);

	virtual void mix(AudioFrame* p_buffer, float p_rate_scale, int p_frames);

	ModularSynthesizerPlayback();
};
