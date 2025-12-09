#pragma once

#include <singleton/wbb_input.h>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/center_container.hpp>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/h_box_container.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {

class UiMenuWbbConnectPrompt : public MarginContainer {
	GDCLASS(UiMenuWbbConnectPrompt, MarginContainer)

private:
	enum WbbPromptState {
		Init,
		Failed,
		Success,
	};

	const String TEX_INFOGRAPHIC_A_PATH = "res://assets/ui/tex_infographic_wbb_battery_cover.svg";
	const String TEX_INFOGRAPHIC_B_PATH = "res://assets/ui/tex_infographic_wbb_sync_button.svg";

	WbbInput *wbb_input;
	WbbPromptState st;

	MarginContainer *content_margin;
	VBoxContainer *content_vbox;
	ColorRect *bgfill;
	ColorRect *fgfill;
	double t_fgfill;
	HBoxContainer *infographic_hbox;
	TextureRect *infographic_a;
	TextureRect *infographic_b;
	RichTextLabel *info_label;
	RichTextLabel *status_label;
	CenterContainer *button_center;
	Button *connect_button;
	RichTextLabel *connect_label;
	Button *exit_button;

	void setup_layout();
	void complete();

protected:
	static void _bind_methods();

public:
	UiMenuWbbConnectPrompt();
	~UiMenuWbbConnectPrompt();

	void _ready() override;
	void _process(double delta) override;

	void connect_wbb();
};

} //namespace godot