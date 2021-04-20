#include "node_data.h"

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
	BIND_ENUM_CONSTANT(NODE_SINE_WAVE);
	BIND_ENUM_CONSTANT(NODE_ADD);
	BIND_ENUM_CONSTANT(NODE_MULTIPLY);
	BIND_ENUM_CONSTANT(NODE_PARAMETER);
	BIND_ENUM_CONSTANT(NODE_COMMENT);

	ClassDB::bind_method(D_METHOD("set_params"), &NodeData::set_params);
	ClassDB::bind_method(D_METHOD("get_params"), &NodeData::get_params);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "params"), "set_params", "get_params");
}
