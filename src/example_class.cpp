#include "example_class.h"

#define MAX_WIIMOTES 2

void ExampleClass::_bind_methods() {
}

void ExampleClass::_ready() {
	wiimotes = wiiuse_init(MAX_WIIMOTES);
}

void ExampleClass::_process(double delta) {
}

void ExampleClass::_physics_process(double delta) {
	int found = get_found_motes();
	print_line("found: ", found);

	int connected = get_connected_motes();
	print_line("connected: ", connected);
}

int32_t ExampleClass::get_found_motes() {
	if (wiimotes == nullptr) {
		return 0;
	}
	int32_t found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	return found;
}

int32_t ExampleClass::get_connected_motes() {
	if (wiimotes == nullptr) {
		return 0;
	}
	int32_t connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	return connected;
}

/// @brief
/// @return first balance board or nullptr
wii_board_t *ExampleClass::get_balance_board() {
	int32_t connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	if (!connected) {
		return nullptr;
	}
	for (int i = 0; i < connected; i++) {
		wiimote *wm = wiimotes[i];
		if (wm->exp.type == EXP_WII_BOARD) {
			return (wii_board_t *)&wm->exp.wb;
		}
	}
	return nullptr;
}

Vector2 ExampleClass::get_axis() {
	wii_board_t *wb = get_balance_board();
	if (!wb) {
		return Vector2();
	}

	float total = wb->tl + wb->tr + wb->bl + wb->br;
	float x = ((wb->tr + wb->br) / total) * 2 - 1;
	float y = ((wb->tl + wb->tr) / total) * 2 - 1;

	return Vector2(x, y);
}
