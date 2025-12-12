#include "ui_menu_wbb_connect_prompt.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>

namespace godot {

void UiMenuWbbConnectPrompt::_bind_methods() {
	ADD_SIGNAL(MethodInfo("completed"));
}

UiMenuWbbConnectPrompt::UiMenuWbbConnectPrompt() {
}

UiMenuWbbConnectPrompt::~UiMenuWbbConnectPrompt() {
}

void UiMenuWbbConnectPrompt::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	wbb_input = WbbInput::get_singleton();

	setup_layout();
}

void UiMenuWbbConnectPrompt::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (Input::get_singleton()->is_key_pressed(KEY_ESCAPE)) {
		complete();
		return;
	}

	t_fgfill += delta * 2.;
	if (t_fgfill < 1.) {
		fgfill->set_modulate(Color::from_hsv(0., 0., 1., 1. - t_fgfill));
	} else {
		fgfill->hide();
	}

	Vector2 size = get_size();
	content_margin->add_theme_constant_override("margin_left", size.x * .1);
	content_margin->add_theme_constant_override("margin_right", size.x * .1);
	content_margin->add_theme_constant_override("margin_top", size.y * .1);
	content_margin->add_theme_constant_override("margin_bottom", size.y * .1);

	switch (st) {
		case WbbPromptState::Init: {
			status_label->set_text(" ");
			connect_button->show();
			exit_button->hide();
			break;
		}

		case WbbPromptState::Failed: {
			status_label->set_text("[color=black]Could not find a balance board.");
			connect_button->show();
			exit_button->hide();
			break;
		}

		case WbbPromptState::Success: {
			status_label->set_text("[color=black]Balance board connected!");
			connect_button->hide();
			exit_button->show();
			break;
		}
	}
}

void UiMenuWbbConnectPrompt::connect_wbb() {
	wbb_input->try_connect();
	if (wbb_input->get_balance_board()) {
		st = WbbPromptState::Success;
	} else {
		st = WbbPromptState::Failed;
	}
}

void UiMenuWbbConnectPrompt::setup_layout() {
	set_anchors_and_offsets_preset(PRESET_FULL_RECT);

	bgfill = memnew(ColorRect);
	add_child(bgfill);

	content_margin = memnew(MarginContainer);
	add_child(content_margin);

	content_vbox = memnew(VBoxContainer);
	content_margin->add_child(content_vbox);

	infographic_hbox = memnew(HBoxContainer);
	infographic_hbox->set_v_size_flags(SIZE_EXPAND_FILL);
	content_vbox->add_child(infographic_hbox);

	infographic_a = memnew(TextureRect);
	infographic_a->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	infographic_a->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	Ref<Texture2D> infographic_a_tex = godot::ResourceLoader::get_singleton()->load(TEX_INFOGRAPHIC_A_PATH, "ImageTexture");
	if (infographic_a_tex != nullptr) {
		infographic_a->set_texture(infographic_a_tex);
	}
	infographic_a->set_h_size_flags(SIZE_EXPAND_FILL);
	infographic_hbox->add_child(infographic_a);

	infographic_b = memnew(TextureRect);
	infographic_b->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	infographic_b->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	Ref<Texture2D> infographic_b_tex = godot::ResourceLoader::get_singleton()->load(TEX_INFOGRAPHIC_B_PATH, "ImageTexture");
	if (infographic_b_tex != nullptr) {
		infographic_b->set_texture(infographic_b_tex);
	}
	infographic_b->set_h_size_flags(SIZE_EXPAND_FILL);
	infographic_hbox->add_child(infographic_b);

	info_label = memnew(RichTextLabel);
	info_label->set_use_bbcode(true);
	info_label->set_v_size_flags(SIZE_EXPAND_FILL);
	info_label->set_stretch_ratio(.3);
	info_label->set_vertical_alignment(VERTICAL_ALIGNMENT_TOP);
	info_label->set_text("[color=dimgray]Connect your Wii balance board now.\nMake sure that Bluetooth is enabled. Click the button below and then press the Sync button on the balance board.");
	content_vbox->add_child(info_label);

	status_label = memnew(RichTextLabel);
	status_label->set_use_bbcode(true);
	status_label->set_fit_content(true);
	content_vbox->add_child(status_label);

	button_center = memnew(CenterContainer);
	content_vbox->add_child(button_center);

	connect_button = memnew(Button);
	connect_button->set_text("Connect");
	connect_button->connect("pressed", callable_mp(this, &UiMenuWbbConnectPrompt::connect_wbb));
	button_center->add_child(connect_button);

	exit_button = memnew(Button);
	exit_button->set_text("Ok");
	exit_button->hide();
	exit_button->connect("pressed", callable_mp(this, &UiMenuWbbConnectPrompt::complete));
	button_center->add_child(exit_button);

	fgfill = memnew(ColorRect);
	add_child(fgfill);
}

void UiMenuWbbConnectPrompt::complete() {
	emit_signal("completed");
}

} //namespace godot