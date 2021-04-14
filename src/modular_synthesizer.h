#pragma once

#include "servers/audio/audio_stream.h"

#include "node_data.h"

class ModularSynthesizer : public AudioStream {
	GDCLASS(ModularSynthesizer, AudioStream);

	float length = 0;

	Dictionary nodes;
	Array connections;
	Ref<NodeData> output;

protected:
	static void _bind_methods();

public:
	void set_nodes(Dictionary value) { nodes = value; }
	Dictionary get_nodes() const { return nodes; }
	void set_connections(Array value) { connections = value; }
	Array get_connections() const { return connections; }
	void set_output(Ref<NodeData> value) { output = value; }
	Ref<NodeData> get_output() const { return output; }

	virtual float get_length() const { return length; }
	void set_length(float value) { length = value; }

	virtual Ref<AudioStreamPlayback> instance_playback();
	virtual String get_stream_name() const { return "ModularSynth"; };
};
