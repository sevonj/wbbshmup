#include "wbb_input.h"

#define MAX_WIIMOTES 1

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

void WbbInput::poll() {
	if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
		/*
		 *	This happens if something happened on any wiimote.
		 *	So go through each one and check if anything happened.
		 */
		int i = 0;
		for (; i < MAX_WIIMOTES; ++i) {
			wiimote *wm = wiimotes[i];
			switch (wm->event) {
				case WIIUSE_EVENT: {
					if (wm->exp.type != EXP_WII_BOARD) {
						// You are not a balance board; we don't care what you have to say.
						break;
					}
					wii_board_t *wb = (wii_board_t *)&wm->exp.wb;

					tl = wb->tl;
					tr = wb->tr;
					bl = wb->bl;
					br = wb->br;

					break;
				}

				default:
					break;
			}
		}
	}
}

void WbbInput::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	wiimotes = wiiuse_init(MAX_WIIMOTES);

	int found = get_found_motes();
	print_line("found: ", found);

	int connected = get_connected_motes();
	print_line("connected: ", connected);

	for (int i = 0; i < MAX_WIIMOTES; i++) {
		wiimote *wm = wiimotes[i];
		print_line("mote exp: ", wm->exp.type);
		if (wm->exp.type == EXP_WII_BOARD) {
			wiiuse_set_leds(wiimotes[i], 0xf0);
		}
	}
}

void WbbInput::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
}

void WbbInput::_physics_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	// Polling is done twice on purpose. Otherwise there's like a whole second of input lag.
	// I assume physics ticks are too slow, causing events piling up?
	poll();
	poll();
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
	for (int i = 0; i < MAX_WIIMOTES; i++) {
		wiimote *wm = wiimotes[i];
		if (wm->exp.type == EXP_WII_BOARD) {
			return (wii_board_t *)&wm->exp.wb;
		}
	}
	return nullptr;
}

float deadzone(double value) {
	if (value > 0) {
		value -= 3;
		return Math::max(value, 0.0);
	} else {
		value += 3;
		return Math::min(value, 0.0);
	}
}

Vector2 WbbInput::get_axis() {
	float total = tl + tr + bl + br;

	float x = deadzone(tr + br) - deadzone(tl + bl);
	float y = deadzone(br + bl) - deadzone(tr + tl);

	print_line("Weight: ", total, " kg @ (", x, ", ", y, ")\n");
	print_line("Interpolated weight: TL", tl, "  TR:", tr, "   BL", bl, "  BR:", br);
	return Vector2(x, y) / 70;
}
