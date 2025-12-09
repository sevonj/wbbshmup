#include "main_menu.h"

#include <godot_cpp/classes/scene_tree.hpp>

namespace godot {

void MainMenu::_bind_methods() {
}

MainMenu::MainMenu() {
	wbb_connect_prompt = nullptr;
	start_game_button = nullptr;
}

MainMenu::~MainMenu() {
}

void MainMenu::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	setup_layout();
}

void MainMenu::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
}

void MainMenu::setup_layout() {
	start_game_button = memnew(Button);
	start_game_button->set_text("Start Game");
	start_game_button->connect("pressed", callable_mp(this, &MainMenu::start_game));
	add_child(start_game_button);

	wbb_connect_prompt = memnew(UiMenuWbbConnectPrompt);
	wbb_connect_prompt->connect("completed", callable_mp(this, &MainMenu::on_wbb_connect_complete));
	add_child(wbb_connect_prompt);
}

void MainMenu::start_game() {
	SceneTree *scene_tree = (SceneTree *)Engine::get_singleton()->get_main_loop();
	scene_tree->change_scene_to_file("res://scenes/stage_test.tscn");
}

void MainMenu::on_wbb_connect_complete() {
	wbb_connect_prompt->queue_free();
	wbb_connect_prompt = nullptr;
}

} //namespace godot