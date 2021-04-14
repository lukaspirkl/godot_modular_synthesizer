#pragma once

#include "servers/audio/audio_stream.h"

#include "../thirdparty/tonic/src/Tonic.h"

class ModularSynthesizerPlayback : public AudioStreamPlayback {
	GDCLASS(ModularSynthesizerPlayback, AudioStreamPlayback);
	friend class ModularSynthesizer;

	bool active;
	ModularSynthesizer* res;
	uint64_t pos;
	Tonic::Synth synth;

	String _get_node_connected_to(String name, int index);
	Tonic::Generator _create_generator(String name);

protected:
	static void _bind_methods();

public:
	virtual void start(float p_from_pos = 0.0);
	virtual void stop();
	virtual bool is_playing() const;

	virtual int get_loop_count() const; //times it looped

	virtual float get_playback_position() const;
	virtual void seek(float p_time);

	virtual void mix(AudioFrame* p_buffer, float p_rate_scale, int p_frames);

	ModularSynthesizerPlayback();
};
