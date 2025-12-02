#include "wbb_input.h"

#define MAX_WIIMOTES 2

WbbInput *WbbInput::singleton = nullptr;
wiimote **WbbInput::wiimotes = nullptr;

void WbbInput::_bind_methods() {
}

WbbInput *WbbInput::get_singleton() {
	if (Engine::get_singleton()->is_editor_hint()) {
		print_error("Something loaded WbbInput in editor!");
	}
	if (unlikely(singleton == nullptr)) {
		singleton = memnew(WbbInput);
		SceneTree *scene_tree = (SceneTree *)Engine::get_singleton()->get_main_loop();
		scene_tree->get_current_scene()->add_child(singleton);
	}
	return singleton;
}

WbbInput::WbbInput() {
	CRASH_COND(singleton != nullptr);
	singleton = this;
	wiimotes = nullptr;
}

WbbInput::~WbbInput() {
	CRASH_COND(singleton == nullptr);
	singleton = nullptr;
}

void WbbInput::_ready() {
	wiimotes = wiiuse_init(MAX_WIIMOTES);
}

void WbbInput::_process(double delta) {
}

void WbbInput::_physics_process(double delta) {
	int found = get_found_motes();
	print_line("found: ", found);

	int connected = get_connected_motes();
	print_line("connected: ", connected);
}

int32_t WbbInput::get_found_motes() {
	if (wiimotes == nullptr) {
		return 0;
	}
	int32_t found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	return found;
}

int32_t WbbInput::get_connected_motes() {
	if (wiimotes == nullptr) {
		return 0;
	}
	int32_t connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	return connected;
}

/// @brief
/// @return first balance board or nullptr
wii_board_t *WbbInput::get_balance_board() {
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

Vector2 WbbInput::get_axis() {
	wii_board_t *wb = get_balance_board();
	if (!wb) {
		return Vector2();
	}

	float total = wb->tl + wb->tr + wb->bl + wb->br;
	float x = ((wb->tr + wb->br) / total) * 2 - 1;
	float y = ((wb->tl + wb->tr) / total) * 2 - 1;

	return Vector2(x, y);
}
