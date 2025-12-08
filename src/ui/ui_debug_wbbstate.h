#pragma once

#include <singleton/wbb_input.h>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>

namespace godot {

class UiDebugWbbstate : public PanelContainer {
	GDCLASS(UiDebugWbbstate, PanelContainer)

private:
	Label *label;
	WbbInput *wbb_input;

	void setup_layout();

protected:
	static void _bind_methods();

public:
	UiDebugWbbstate();
	~UiDebugWbbstate();

	void _ready() override;
	void _process(double delta) override;
};

} //namespace godot