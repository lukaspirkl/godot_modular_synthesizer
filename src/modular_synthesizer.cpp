#include "modular_synthesizer.h"

using namespace Tonic;


void NodeData::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_position"), &NodeData::set_position);
	ClassDB::bind_method(D_METHOD("get_position"), &NodeData::get_position);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "position"), "set_position", "get_position");

	ClassDB::bind_method(D_METHOD("set_type"), &NodeData::set_type);
	ClassDB::bind_method(D_METHOD("get_type"), &NodeData::get_type);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "type"), "set_type", "get_type");
	BIND_ENUM_CONSTANT(NODE_SPECIAL);
	BIND_ENUM_CONSTANT(NODE_CONSTANT);

	ClassDB::bind_method(D_METHOD("set_params"), &NodeData::set_params);
	ClassDB::bind_method(D_METHOD("get_params"), &NodeData::get_params);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "params"), "set_params", "get_params");
}



void ConnectionData::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_from"), &ConnectionData::set_from);
	ClassDB::bind_method(D_METHOD("get_from"), &ConnectionData::get_from);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "from"), "set_from", "get_from");
	ClassDB::bind_method(D_METHOD("set_from_index"), &ConnectionData::set_from_index);
	ClassDB::bind_method(D_METHOD("get_from_index"), &ConnectionData::get_from_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "from_index"), "set_from_index", "get_from_index");
	ClassDB::bind_method(D_METHOD("set_to"), &ConnectionData::set_to);
	ClassDB::bind_method(D_METHOD("get_to"), &ConnectionData::get_to);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "to"), "set_to", "get_to");
	ClassDB::bind_method(D_METHOD("set_to_index"), &ConnectionData::set_to_index);
	ClassDB::bind_method(D_METHOD("get_to_index"), &ConnectionData::get_to_index);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "to_index"), "set_to_index", "get_to_index");
}

ConnectionData::ConnectionData()
{
}

ConnectionData::ConnectionData(const String& p_from, int p_from_index, const String& p_to, int p_to_index)
{
	from = p_from;
	from_index = p_from_index;
	to = p_to;
	to_index = p_to_index;
}


Ref<AudioStreamPlayback> ModularSynthesizer::instance_playback() {

	Ref<ModularSynthesizerPlayback> playback;
	playback.instance();
	playback->res = this;
	return playback;
}
String ModularSynthesizer::get_stream_name() const {

	return "ModularSynth";
}

float ModularSynthesizer::get_length() const {
	return 0;
}

void ModularSynthesizer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_volume"), &ModularSynthesizer::set_volume);
	ClassDB::bind_method(D_METHOD("get_volume"), &ModularSynthesizer::get_volume);
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "volume"), "set_volume", "get_volume");

	ClassDB::bind_method(D_METHOD("set_nodes"), &ModularSynthesizer::set_nodes);
	ClassDB::bind_method(D_METHOD("get_nodes"), &ModularSynthesizer::get_nodes);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "nodes"), "set_nodes", "get_nodes");

	ClassDB::bind_method(D_METHOD("set_connections"), &ModularSynthesizer::set_connections);
	ClassDB::bind_method(D_METHOD("get_connections"), &ModularSynthesizer::get_connections);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "connections"), "set_connections", "get_connections");

	ClassDB::bind_method(D_METHOD("set_output"), &ModularSynthesizer::set_output);
	ClassDB::bind_method(D_METHOD("get_output"), &ModularSynthesizer::get_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output"), "set_output", "get_output");
}

ModularSynthesizer::ModularSynthesizer() {
	volume = 0.3f;
}

////////////////

void ModularSynthesizerPlayback::start(float p_from_pos) {
	active = true;
	mixed = 0.0;
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

	Generator output = FixedValue(0);

	Map<String, Generator> generators;
	Dictionary nodes = res->get_nodes();
	for (const Variant* key = nodes.next(); key != NULL; key = nodes.next(key))
	{
		Ref<NodeData> data = nodes[*key];
		switch (data->get_type())
		{
		case NodeData::NodeType::NODE_CONSTANT:
			generators.insert(*key, FixedValue(data->get_params()["value"]));
			break;
		default:
			continue;
		}
	}

	for (size_t i = 0; i < res->get_connections().size(); i++)
	{
		Ref<ConnectionData> c = res->get_connections().get(i);
		
		// Find signal destination
		Generator destination = (c->get_to() == "OutputNode") ? output : generators[c->get_to()];
		
		// Add sum signal source with destinatioln
		destination = destination + generators[c->get_from()];
		
		// Save new generator into destination
		if (c->get_to() == "OutputNode")
		{
			output = destination;
		}
		else
		{
			generators[c->get_to()] = destination;
		}
	}

	Generator vol = FixedValue(res->get_volume());
	Generator tone = SineWave().freq(output) * vol;

	synth.setOutputGen(tone);
}

void ModularSynthesizerPlayback::stop() {
	active = false;
}
bool ModularSynthesizerPlayback::is_playing() const {

	return active; //always playing, can't be stopped
}

int ModularSynthesizerPlayback::get_loop_count() const {
	return 0;
}

float ModularSynthesizerPlayback::get_playback_position() const {
	return mixed;
}
void ModularSynthesizerPlayback::seek(float p_time) {
	//no seek possible
}

void ModularSynthesizerPlayback::mix(AudioFrame* p_buffer, float p_rate_scale, int p_frames) {

	synth.fillBufferOfFloats((float*)p_buffer, p_frames, 2);
}

void ModularSynthesizerPlayback::_bind_methods() {
}

ModularSynthesizerPlayback::ModularSynthesizerPlayback() {

	res = NULL;
	active = false;
	mixed = 0;
	pos = 0;
}
