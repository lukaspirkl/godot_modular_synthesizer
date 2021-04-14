#include "connection_data.h"

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
