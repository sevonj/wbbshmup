#pragma once

#include <ui/ui_menu_wbb_connect_prompt.h>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class MainMenu : public Node {
	GDCLASS(MainMenu, Node)

private:
	UiMenuWbbConnectPrompt *wbb_connect_prompt;

	void setup_layout();

	void on_wbb_connect_complete();

protected:
	static void _bind_methods();

public:
	MainMenu();
	~MainMenu();

	void _ready() override;
	void _process(double delta) override;
};

} //namespace godot