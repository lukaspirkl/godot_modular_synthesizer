#pragma once

#include "core/resource.h"

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
