#pragma once

#include <wbb_input.h>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>

namespace godot {

class UiDebugWbbstatus : public PanelContainer {
	GDCLASS(UiDebugWbbstatus, PanelContainer)

	static constexpr double AXIS_BOX_SIZE = 128.;

	Label *label = nullptr;
	WbbInput *wbb_input = nullptr;
	ColorRect *axis_box = nullptr;

protected:
	static void _bind_methods();

public:
	UiDebugWbbstatus() = default;
	~UiDebugWbbstatus() override = default;

	void _ready() override;
	void _process(double delta) override;

private:
	void setup_layout();
	void debug_draw_axis();
};

} //namespace godot