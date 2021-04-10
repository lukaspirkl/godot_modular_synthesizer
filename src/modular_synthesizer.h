#pragma once

#include "core/ring_buffer.h"
#include "servers/audio/audio_stream.h"

#include "../thirdparty/tonic/src/Tonic.h"

class ModularSynthesizer : public AudioStream {
	GDCLASS(ModularSynthesizer, AudioStream);

protected:
	static void _bind_methods();

public:
	virtual Ref<AudioStreamPlayback> instance_playback();
	virtual String get_stream_name() const;

	virtual float get_length() const;
	ModularSynthesizer();
};

class ModularSynthesizerPlayback : public AudioStreamPlayback {

	GDCLASS(ModularSynthesizerPlayback, AudioStreamPlayback);
	friend class ModularSynthesizer;
	bool active;
	float mixed;
	ModularSynthesizer *generator;
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

	ModularSynthesizerPlayback();
};
