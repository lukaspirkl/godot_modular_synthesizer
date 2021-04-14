#pragma once

#include "core/resource.h"


class NodeData : public Resource {
	GDCLASS(NodeData, Resource);

public:
	enum NodeType {
		NODE_SPECIAL = 0,
		NODE_CONSTANT = 1,
		NODE_SINE_WAVE = 2,
		NODE_ADD = 100,
		NODE_MULTIPLY = 101,
	};

private:
	Vector2 position = Vector2(0, 0);
	NodeType type = NODE_SPECIAL;
	Dictionary params;

protected:
	static void _bind_methods();

public:
	void set_position(Vector2 value) { position = value; }
	Vector2 get_position() { return position; }
	void set_type(NodeType value) { type = value; }
	NodeType get_type() { return type; }
	void set_params(Dictionary value) { params = value; }
	Dictionary get_params() { return params; }
};

VARIANT_ENUM_CAST(NodeData::NodeType);
