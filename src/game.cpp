#include "game.h"

#include <godot_cpp/classes/engine.hpp>

namespace godot {

Game *Game::singleton = nullptr;
CameraRig *Game::current_camera = nullptr;
Player *Game::player = nullptr;
Stage *Game::stage = nullptr;

Game *Game::get_singleton() {
	if (unlikely(singleton == nullptr)) {
		singleton = memnew(Game);
	}
	return singleton;
}

Game::Game() {
	CRASH_COND(singleton != nullptr);
	singleton = this;
}

CameraRig *Game::get_current_camera() {
	return current_camera;
}

void Game::set_current_camera(CameraRig *camera) {
	current_camera = camera;
}

Player *Game::get_player() {
	return player;
}

void Game::set_player(Player *value) {
	player = value;
}

Stage *Game::get_stage() {
	if (!stage) {
		print_error("Game::get_stage() - Someone asked for the stage, but it's null!");
	}
	return stage;
}

void Game::set_stage(Stage *value) {
	stage = value;
}

bool Game::is_out_of_bounds(Vector3 position) {
	return !DEFAULT_STAGE_BOUNDS.has_point(position);
}

bool Game::is_out_of_bounds(Node3D *node) {
	return is_out_of_bounds(node->get_global_position());
}

bool Game::is_editor() {
	return Engine::get_singleton()->is_editor_hint();
}

} //namespace godot