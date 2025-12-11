#pragma once

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {

class UiStageBeginScreen : public MarginContainer {
	GDCLASS(UiStageBeginScreen, MarginContainer)

private:
	static constexpr double FADEIN_DURATION = .6;
	static constexpr double DISPLAY_DURATION = 3.;
	static constexpr double FADEOUT_DURATION = .6;

	static constexpr double T_START_DISPLAY = FADEIN_DURATION;
	static constexpr double T_START_FADEOUT = T_START_DISPLAY + DISPLAY_DURATION;
	static constexpr double T_DIE = T_START_FADEOUT + FADEOUT_DURATION;

	ColorRect *bgfill;
	Panel *top_panel;
	Panel *bottom_panel;
	Label *lab_stage_name;
	Label *lab_stage_no;
	Label *lab_stage_desc;
	TextureRect *tex_stage_icon;
	String tex_stage_icon_path;

	double lifetimer;

	void setup_layout();

protected:
	static void _bind_methods();

public:
	UiStageBeginScreen();
	~UiStageBeginScreen();

	void _ready() override;
	void _process(double delta) override;
};

} //namespace godot