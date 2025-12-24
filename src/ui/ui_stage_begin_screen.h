#pragma once

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/panel.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {

class UiStageBeginScreen : public MarginContainer {
	GDCLASS(UiStageBeginScreen, MarginContainer)

	static constexpr double FADEIN_DURATION = .6;
	static constexpr double DISPLAY_DURATION = 3.;
	static constexpr double FADEOUT_DURATION = .6;
	static constexpr const char *DEFAULT_STAGE_ICON_PATH = "res://assets/ui/stage_icons/tex_icon_stage_saturn.png";

	static constexpr double T_START_DISPLAY = FADEIN_DURATION;
	static constexpr double T_START_FADEOUT = T_START_DISPLAY + DISPLAY_DURATION;
	static constexpr double T_DIE = T_START_FADEOUT + FADEOUT_DURATION;

	// --- state, config
	String stage_name = "Stage Name";
	String stage_no = "Stage No.";
	String stage_desc = "Stage Description";
	Ref<Texture2D> stage_icon;
	double lifetimer = 0.0;

	// --- components
	ColorRect *bgfill = nullptr;
	Panel *top_panel = nullptr;
	Panel *bottom_panel = nullptr;
	Label *lab_stage_name = nullptr;
	Label *lab_stage_no = nullptr;
	Label *lab_stage_desc = nullptr;
	TextureRect *tex_stage_icon = nullptr;

protected:
	static void _bind_methods();

public:
	UiStageBeginScreen() = default;
	~UiStageBeginScreen() override = default;

	void set_stage_name(String text);
	void set_stage_no(String text);
	void set_stage_desc(String text);
	void set_stage_icon(Ref<Texture2D> tex);

	void _ready() override;
	void _process(double delta) override;

private:
	void setup_layout();
};

} //namespace godot