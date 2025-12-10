#pragma once

#include <singleton/wbb_input.h>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/panel_container.hpp>

namespace godot {

class UiDebugWbbstatus : public PanelContainer {
	GDCLASS(UiDebugWbbstatus, PanelContainer)

private:
	static constexpr double AXIS_BOX_SIZE = 128.;

	Label *label;
	WbbInput *wbb_input;
	ColorRect *axis_box;

	void setup_layout();
	void debug_draw_axis();

protected:
	static void _bind_methods();

public:
	UiDebugWbbstatus();
	~UiDebugWbbstatus();

	void _ready() override;
	void _process(double delta) override;
};

} //namespace godot