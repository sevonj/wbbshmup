#include "main_menu.h"

namespace godot {

void MainMenu::_bind_methods() {
}

MainMenu::MainMenu() {
	wbb_connect_prompt = nullptr;
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
	wbb_connect_prompt = memnew(UiMenuWbbConnectPrompt);
	wbb_connect_prompt->connect("completed", callable_mp(this, &MainMenu::on_wbb_connect_complete));
	add_child(wbb_connect_prompt);
}

void MainMenu::on_wbb_connect_complete() {
	wbb_connect_prompt->queue_free();
	wbb_connect_prompt = nullptr;
}

} //namespace godot