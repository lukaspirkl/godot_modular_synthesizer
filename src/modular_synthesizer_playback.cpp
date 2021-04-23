#include "modular_synthesizer_playback.h"
#include "modular_synthesizer.h"
#include "connection_data.h"

using namespace Tonic;

void ModularSynthesizerPlayback::_bind_methods() {
	ClassDB::bind_method("resource_changed", &ModularSynthesizerPlayback::resource_changed);
	ClassDB::bind_method("parameter_changed", &ModularSynthesizerPlayback::parameter_changed);
}

void ModularSynthesizerPlayback::set_resource(ModularSynthesizer* p_res)
{
	res = p_res;
	_refresh_synth();
}

void ModularSynthesizerPlayback::resource_changed()
{
	is_res_up_to_date = false;
	_refresh_synth();
}

void ModularSynthesizerPlayback::parameter_changed(const String& p_name, float p_value)
{
	std::wstring ws = p_name.c_str();
	std::string s(ws.begin(), ws.end());
	synth.setParameter(s, p_value);
}

void ModularSynthesizerPlayback::start(float p_from_pos) {
	active = true;
	_refresh_synth();
}

void ModularSynthesizerPlayback::_refresh_synth()
{
	pos = 0;
	if (!is_res_up_to_date)
	{
		for (Map<String, Generator*>::Element* E = generators.back(); E; E = E->prev())
		{
			Generator* gen = E->value();
			if (gen != NULL)
			{
				memdelete(gen);
			}
		}
		generators.clear();

		for (Map<String, ControlGenerator*>::Element* E = control_generators.back(); E; E = E->prev())
		{
			ControlGenerator* gen = E->value();
			if (gen != NULL)
			{
				memdelete(gen);
			}
		}
		control_generators.clear();

		String name = _get_node_connected_to("OutputNode", 0);
		Generator* gen = _get_generator(name);
		synth.setOutputGen(*gen);
		is_res_up_to_date = true;
	}
	else
	{
		synth.forceNewOutput();
	}
}

Tonic::Generator* ModularSynthesizerPlayback::_get_generator(const String& name)
{
	if (generators.has(name))
	{
		return generators[name];
	}

	Generator* gen = _create_generator(name);
	generators.insert(name, gen);
	return gen;
}

Generator* ModularSynthesizerPlayback::_create_generator(const String& name)
{
	if (name == "")
	{
		return memnew(FixedValue(0));
	}

	Ref<NodeData> data = res->get_nodes()[name];
	switch (data->get_type())
	{
	case NodeData::NodeType::NODE_CONSTANT: {
		return memnew(FixedValue(data->get_params()["value"]));
	}
	case NodeData::NodeType::NODE_SINE_WAVE: {
		String freqName = _get_node_connected_to(name, 0);
		SineWave* s = memnew(SineWave());
		if (freqName == "")
		{
			s->freq(data->get_params()["freq"]);
		}
		else
		{
			ControlGenerator* cg = _get_control_generator(freqName);
			if (cg != NULL)
			{
				s->freq(*cg);
			}
			else
			{
				s->freq(*_get_generator(freqName));
			}
		}
		return s;
	}
	case NodeData::NodeType::NODE_ADD: {
		Adder* a = memnew(Adder());
		String name_a = _get_node_connected_to(name, 0);
		if (name_a != "")
		{
			a->input(*_get_generator(name_a));
		}
		String name_b = _get_node_connected_to(name, 1);
		if (name_b != "")
		{
			a->input(*_get_generator(name_b));
		}
		else
		{
			a->input(FixedValue(data->get_params()["value"]));
		}
		return a;
	}
	case NodeData::NodeType::NODE_MULTIPLY: {
		Multiplier* a = memnew(Multiplier());
		String name_a = _get_node_connected_to(name, 0);
		if (name_a != "")
		{
			a->input(*_get_generator(name_a));
		}
		String name_b = _get_node_connected_to(name, 1);
		if (name_b != "")
		{
			a->input(*_get_generator(name_b));
		}
		else
		{
			a->input(FixedValue(data->get_params()["value"]));
		}
		return a;
	}
	case NodeData::NodeType::NODE_ADSR: {
		ADSR* adsr = memnew(ADSR);
		adsr->attack(data->get_params()["a"]);
		adsr->decay(data->get_params()["d"]);
		adsr->sustain(data->get_params()["s"]);
		adsr->release(data->get_params()["r"]);
		adsr->legato(true);
		String name_trig = _get_node_connected_to(name, 0);
		if (name_trig != "")
		{
			adsr->trigger(*_get_control_generator(name_trig));
		}
		return adsr;
	}
	case NodeData::NodeType::NODE_PARAMETER:
	case NodeData::NodeType::NODE_METRONOME:
	case NodeData::NodeType::NODE_SPECIAL:
	case NodeData::NodeType::NODE_COMMENT:
	default:
		return memnew(FixedValue(0));
	}
}

Tonic::ControlGenerator* ModularSynthesizerPlayback::_get_control_generator(const String& name)
{
	if (control_generators.has(name))
	{
		return control_generators[name];
	}

	ControlGenerator* gen = _create_control_generator(name);
	control_generators.insert(name, gen);
	return gen;
}

Tonic::ControlGenerator* ModularSynthesizerPlayback::_create_control_generator(const String& name)
{
	if (name == "")
	{
		return NULL;
	}

	Ref<NodeData> data = res->get_nodes()[name];
	switch (data->get_type())
	{
	case NodeData::NodeType::NODE_PARAMETER: {
		String name = data->get_params()["name"];
		ControlParameter* c = memnew(ControlParameter);
		std::wstring ws = name.c_str();
		std::string s(ws.begin(), ws.end());
		c->name(s);
		c->value(data->get_params()["value"]);
		synth.addParameter(*c);
		return c;
	}
	case NodeData::NodeType::NODE_METRONOME: {
		return memnew(ControlMetro(data->get_params()["bpm"]));
	}
	case NodeData::NodeType::NODE_ADSR:
	case NodeData::NodeType::NODE_CONSTANT:
	case NodeData::NodeType::NODE_SINE_WAVE:
	case NodeData::NodeType::NODE_ADD:
	case NodeData::NodeType::NODE_MULTIPLY:
	case NodeData::NodeType::NODE_SPECIAL:
	case NodeData::NodeType::NODE_COMMENT:
	default:
		return NULL;
	}
}

String ModularSynthesizerPlayback::_get_node_connected_to(const String& name, int index)
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
