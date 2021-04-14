#include "modular_synthesizer_playback.h"
#include "modular_synthesizer.h"
#include "connection_data.h"

using namespace Tonic;

void ModularSynthesizerPlayback::start(float p_from_pos) {
	active = true;
	pos = 0;

	//ControlMetro metro = ControlMetro().bpm(100);
	//ControlGenerator freq = ControlRandom().trigger(metro).min(0).max(1);

	//Generator tone = SquareWaveBL().freq(
	//						 freq * 0.25 + 100 + 400) *
	//				 SineWave().freq(50);

	//ADSR env = ADSR()
	//				   .attack(0.01)
	//				   .decay(0.4)
	//				   .sustain(0)
	//				   .release(0)
	//				   .doesSustain(false)
	//				   .trigger(metro);

	//StereoDelay delay = StereoDelay(3.0f, 3.0f)
	//							.delayTimeLeft(0.5 + SineWave().freq(0.2) * 0.01)
	//							.delayTimeRight(0.55 + SineWave().freq(0.23) * 0.01)
	//							.feedback(0.3)
	//							.dryLevel(0.8)
	//							.wetLevel(0.2);

	//Generator filterFreq = (SineWave().freq(0.01) + 1) * 200 + 225;

	//LPF24 filter = LPF24().Q(2).cutoff(filterFreq);

	//Generator output = ((tone * env) >> filter >> delay) * 0.3;

	
	String name = _get_node_connected_to("OutputNode", 0);
	Generator gen = _create_generator(name);
	synth.setOutputGen(gen);
}

Generator ModularSynthesizerPlayback::_create_generator(String name)
{
	Ref<NodeData> data = res->get_nodes()[name];
	switch (data->get_type())
	{
	case NodeData::NodeType::NODE_CONSTANT: {
		return FixedValue(data->get_params()["value"]);
	}
	case NodeData::NodeType::NODE_SINE_WAVE: {
		SineWave sine = SineWave();
		String freqName = _get_node_connected_to(name, 0);
		if (freqName == "")
		{
			sine.freq(data->get_params()["freq"]);
		}
		else
		{
			sine.freq(_create_generator(freqName));
		}
		return sine;
	}
	case NodeData::NodeType::NODE_ADD: {
		String name_a = _get_node_connected_to(name, 0);
		String name_b = _get_node_connected_to(name, 1);
		Generator gen_a = name_a == "" ? FixedValue(0) : _create_generator(name_a);
		Generator gen_b = name_b == "" ? FixedValue(data->get_params()["value"]) : _create_generator(name_b);
		return gen_a + gen_b;
	}
	case NodeData::NodeType::NODE_MULTIPLY: {
		String name_a = _get_node_connected_to(name, 0);
		String name_b = _get_node_connected_to(name, 1);
		Generator gen_a = name_a == "" ? FixedValue(0) : _create_generator(name_a);
		Generator gen_b = name_b == "" ? FixedValue(data->get_params()["value"]) : _create_generator(name_b);
		return gen_a * gen_b;
	}
	}
	return FixedValue(0);
}

String ModularSynthesizerPlayback::_get_node_connected_to(String name, int index)
{
	for (size_t i = 0; i < res->get_connections().size(); i++)
	{
		Ref<ConnectionData> c = res->get_connections().get(i);
		if (c->get_to() == name && c->get_to_index() == index)
		{
			return c->get_from();
		}
	}
	return "";
}

void ModularSynthesizerPlayback::stop() {
	active = false;
}
bool ModularSynthesizerPlayback::is_playing() const {

	return active;
}

int ModularSynthesizerPlayback::get_loop_count() const {
	return 0;
}

float ModularSynthesizerPlayback::get_playback_position() const {
	return pos / Tonic::sampleRate();
}

void ModularSynthesizerPlayback::seek(float p_time) {
	//no seek possible
}

void ModularSynthesizerPlayback::mix(AudioFrame* p_buffer, float p_rate_scale, int p_frames) {
	synth.fillBufferOfFloats((float*)p_buffer, p_frames, 2);
	pos += p_frames;

	if (res->get_length() != 0 && pos >= res->get_length() * Tonic::sampleRate())
	{
		active = false;
	}
}

void ModularSynthesizerPlayback::_bind_methods() {
}

ModularSynthesizerPlayback::ModularSynthesizerPlayback() {
	res = NULL;
	active = false;
	pos = 0;
}
