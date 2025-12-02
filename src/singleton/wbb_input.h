#pragma once

#include <wiiuse.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class WbbInput : public Node {
	GDCLASS(WbbInput, Node)

private:
	static WbbInput *singleton;

	static wiimote **wiimotes;

protected:
	static void _bind_methods();

public:
	static WbbInput *get_singleton();
	WbbInput();
	~WbbInput();

	void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

	int32_t get_found_motes();
	int32_t get_connected_motes();
	wii_board_t *get_balance_board();
	Vector2 get_axis();
};
