#include "modular_synthesizer.h"
#include "modular_synthesizer_playback.h"

using namespace Tonic;

Ref<AudioStreamPlayback> ModularSynthesizer::instance_playback() {

	Ref<ModularSynthesizerPlayback> playback;
	playback.instance();
	playback->res = this;
	this->connect("changed", playback.ptr(), "resource_changed");
	return playback;
}

void ModularSynthesizer::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_length"), &ModularSynthesizer::set_length);
	// get_length is already registered by base class
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "length"), "set_length", "get_length");

	ClassDB::bind_method(D_METHOD("set_nodes"), &ModularSynthesizer::set_nodes);
	ClassDB::bind_method(D_METHOD("get_nodes"), &ModularSynthesizer::get_nodes);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "nodes", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_nodes", "get_nodes");

	ClassDB::bind_method(D_METHOD("set_connections"), &ModularSynthesizer::set_connections);
	ClassDB::bind_method(D_METHOD("get_connections"), &ModularSynthesizer::get_connections);
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "connections", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_connections", "get_connections");

	ClassDB::bind_method(D_METHOD("set_output"), &ModularSynthesizer::set_output);
	ClassDB::bind_method(D_METHOD("get_output"), &ModularSynthesizer::get_output);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "output", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_output", "get_output");

	ClassDB::bind_method(D_METHOD("set_offset"), &ModularSynthesizer::set_offset);
	ClassDB::bind_method(D_METHOD("get_offset"), &ModularSynthesizer::get_offset);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "offset", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "set_offset", "get_offset");
}
