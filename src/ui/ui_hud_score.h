#pragma once

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>

namespace godot {

class UiHudScore : public PanelContainer {
	GDCLASS(UiHudScore, PanelContainer)

	Label *lab_score = nullptr;
	Label *lab_mult = nullptr;

protected:
	static void _bind_methods();

public:
	UiHudScore() = default;
	~UiHudScore() override = default;

	void _ready() override;
	void _process(double delta) override;

private:
	void setup_layout();
};

} //namespace godot