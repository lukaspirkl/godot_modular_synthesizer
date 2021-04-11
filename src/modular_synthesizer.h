#pragma once

#include "core/ring_buffer.h"
#include "servers/audio/audio_stream.h"

#include "../thirdparty/tonic/src/Tonic.h"

class NodeData : public Resource {
	GDCLASS(NodeData, Resource);

	Vector2 position = Vector2(0, 0);
	int type = -1;

protected:
	static void _bind_methods();

public:
	void set_position(Vector2 value) { position = value; }
	Vector2 get_position() { return position; }
	void set_type(int value) { type = value; }
	int get_type() { return type; }
};

class ConnectionData : public Resource {
	GDCLASS(ConnectionData, Resource);

	String from = "";
	int from_index = 0;
	String to = "";
	int to_index = 0;

protected:
	static void _bind_methods();

public:
	void set_from(String value) { from = value; }
	String get_from() { return from; }
	void set_from_index(int value) { from_index = value; }
	int get_from_index() { return from_index; }
	void set_to(String value) { to = value; }
	String get_to() { return to; }
	void set_to_index(int value) { to_index = value; }
	int get_to_index() { return to_index; }

	ConnectionData();
	ConnectionData(const String& p_from, int p_from_index, const String& p_to, int p_to_index);
};

class ModularSynthesizer : public AudioStream {
	GDCLASS(ModularSynthesizer, AudioStream);

	float frequency;
	float volume;

	Dictionary nodes;
	Array connections;
	Ref<NodeData> output;

protected:
	static void _bind_methods();

public:
	void set_frequency(float f) { frequency = f; }
	float get_frequency() const { return frequency; }
	void set_volume(float f) { volume = f; }
	float get_volume() const { return volume; }
	void set_nodes(Dictionary value) { nodes = value; }
	Dictionary get_nodes() const { return nodes; }
	void set_connections(Array value) { connections = value; }
	Array get_connections() { return connections; }
	void set_output(Ref<NodeData> value) { output = value; }
	Ref<NodeData> get_output() { return output; }

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
	ModularSynthesizer* generator;
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

	virtual void mix(AudioFrame* p_buffer, float p_rate_scale, int p_frames);

	ModularSynthesizerPlayback();
};
