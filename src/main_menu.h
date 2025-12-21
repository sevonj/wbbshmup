#pragma once

#include <ui/ui_menu_wbb_connect_prompt.h>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class MainMenu : public Node {
	GDCLASS(MainMenu, Node)

	// --- components
	UiMenuWbbConnectPrompt *wbb_connect_prompt = nullptr;
	Button *start_game_button = nullptr;

protected:
	static void _bind_methods();

public:
	MainMenu() = default;
	~MainMenu() override = default;

	void _ready() override;
	void _process(double delta) override;

private:
	void setup_layout();

	void start_game();
	void on_wbb_connect_complete();
};

} //namespace godot