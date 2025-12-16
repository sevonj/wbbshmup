#include "game.h"

#include <godot_cpp/classes/engine.hpp>

namespace godot {

Game *Game::singleton = nullptr;
CameraRig *Game::current_camera = nullptr;
Player *Game::player = nullptr;
Stage *Game::stage = nullptr;

Game *Game::get_singleton() {
	if (unlikely(!singleton)) {
		singleton = memnew(Game);
	}
	return singleton;
}

Game::Game() {
	CRASH_COND(singleton);
	singleton = this;
}

bool Game::is_out_of_bounds(Vector3 position) {
	return !DEFAULT_STAGE_BOUNDS.has_point(position);
}

} //namespace godot