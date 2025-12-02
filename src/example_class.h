#pragma once

#include <wiiuse.h>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class ExampleClass : public Node {
	GDCLASS(ExampleClass, Node)

	wiimote **wiimotes;

protected:
	static void _bind_methods();

public:
	ExampleClass() {
		wiimotes = nullptr;
	};
	~ExampleClass() {};

	void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

	void print_type(const Variant &p_variant) const;
	uint32_t get_found_motes();
	uint32_t get_connected_motes();
};
