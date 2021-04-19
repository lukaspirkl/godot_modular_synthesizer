#include "modular_synthesizer_playback.h"
#include "modular_synthesizer.h"
#include "connection_data.h"

using namespace Tonic;

void ModularSynthesizerPlayback::_bind_methods() {
	ClassDB::bind_method("resource_changed", &ModularSynthesizerPlayback::resource_changed);
}

void ModularSynthesizerPlayback::resource_changed()
{
	is_res_up_to_date = false;
}

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

	if (!is_res_up_to_date)
	{
		for (Map<String, Generator*>::Element* E = gens.back(); E; E = E->prev())
		{
			Generator* gen = E->value();
			memdelete(gen);
		}
		gens.clear();

		String name = _get_node_connected_to("OutputNode", 0);
		Generator* gen = _create_generator(name);
		synth.setOutputGen(*gen);
		is_res_up_to_date = true;
	}
	else
	{
		synth.forceNewOutput();
	}
}

Generator* ModularSynthesizerPlayback::_create_generator(String name)
{
	if (gens.has(name))
	{
		return gens[name];
	}

	Generator* gen;

	if (name == "")
	{
		gen = memnew(FixedValue(0));
		gens.insert(name, gen);
		return gen;
	}

	Ref<NodeData> data = res->get_nodes()[name];
	switch (data->get_type())
	{
	case NodeData::NodeType::NODE_CONSTANT: {
		gen = memnew(FixedValue(data->get_params()["value"]));
	} break;
	case NodeData::NodeType::NODE_SINE_WAVE: {
		String freqName = _get_node_connected_to(name, 0);
		SineWave* s = memnew(SineWave());
		gen = s;
		if (freqName == "")
		{
			s->freq(data->get_params()["freq"]);
		}
		else
		{
			s->freq(*_create_generator(freqName));
		}
	} break;
	case NodeData::NodeType::NODE_ADD: {
		Adder* a = memnew(Adder());
		gen = a;

		String name_a = _get_node_connected_to(name, 0);
		if (name_a != "")
		{
			a->input(*_create_generator(name_a));
		}

		String name_b = _get_node_connected_to(name, 1);
		if (name_b != "")
		{
			a->input(*_create_generator(name_b));
		}
		else
		{
			a->input(FixedValue(data->get_params()["value"]));
		}
	} break;
	case NodeData::NodeType::NODE_MULTIPLY: {
		Multiplier* a = memnew(Multiplier());
		gen = a;

		String name_a = _get_node_connected_to(name, 0);
		if (name_a != "")
		{
			a->input(*_create_generator(name_a));
		}

		String name_b = _get_node_connected_to(name, 1);
		if (name_b != "")
		{
			a->input(*_create_generator(name_b));
		}
		else
		{
			a->input(FixedValue(data->get_params()["value"]));
		}
	} break;
	case NodeData::NodeType::NODE_SPECIAL:
	case NodeData::NodeType::NODE_COMMENT:
		break;
	}

	gens.insert(name, gen);
	return gen;
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

ModularSynthesizerPlayback::ModularSynthesizerPlayback() {
	res = NULL;
	active = false;
	pos = 0;
}
