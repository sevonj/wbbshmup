#include "ui_stage_begin_screen.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/viewport.hpp>

namespace godot {

void UiStageBeginScreen::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_stage_name"), &UiStageBeginScreen::set_stage_name);
	ClassDB::bind_method(D_METHOD("set_stage_no"), &UiStageBeginScreen::set_stage_no);
	ClassDB::bind_method(D_METHOD("set_stage_desc"), &UiStageBeginScreen::set_stage_desc);
	ClassDB::bind_method(D_METHOD("set_stage_icon"), &UiStageBeginScreen::set_stage_icon);
}

void UiStageBeginScreen::set_stage_name(String text) {
	stage_name = text;
	if (lab_stage_name) {
		lab_stage_name->set_text(stage_name);
	}
}

void UiStageBeginScreen::set_stage_no(String text) {
	stage_no = text;
	if (lab_stage_no) {
		lab_stage_no->set_text(stage_no);
	}
}

void UiStageBeginScreen::set_stage_desc(String text) {
	stage_desc = text;
	if (lab_stage_desc) {
		lab_stage_desc->set_text(stage_desc);
	}
}

void UiStageBeginScreen::set_stage_icon(Ref<Texture2D> tex) {
	stage_icon = tex;
	if (tex_stage_icon) {
	}
}

void UiStageBeginScreen::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (!stage_icon.is_valid()) {
		stage_icon = ResourceLoader::get_singleton()->load(DEFAULT_STAGE_ICON_PATH, "ImageTexture");
	}
	setup_layout();
}

void UiStageBeginScreen::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	lifetimer += delta;

	double t_anim; // ranges from -1 to 1. 0 is fully shown position.

	if (lifetimer < T_START_DISPLAY) {
		// Fadein
		t_anim = lifetimer / FADEIN_DURATION - 1.;
	} else if (lifetimer < T_START_FADEOUT) {
		// Display
		t_anim = 0.;
	} else if (lifetimer < T_DIE) {
		// Fadeout
		t_anim = (lifetimer - T_START_FADEOUT) / FADEOUT_DURATION;
	} else {
		queue_free();
		return;
	}

	Viewport *vp = get_viewport();
	Vector2 size = vp->get_visible_rect().get_size();

	Vector2 top_size = Vector2(1.2, 0.2) * size;
	Vector2 top_pos = Vector2(-0.1, 0.1) * size;
	Vector2 bottom_size = Vector2(1.2, 0.2) * size;
	Vector2 bottom_pos = Vector2(-0.1, 0.7) * size;

	top_pos += Vector2(size.x * 1.3, 0) * t_anim;
	bottom_pos -= Vector2(size.x * 1.3, 0) * t_anim;

	top_panel->set_size(top_size);
	top_panel->set_position(top_pos);
	bottom_panel->set_size(bottom_size);
	bottom_panel->set_position(bottom_pos);

	Vector2 lab_stage_name_off = Vector2(0.2, 0.5) * top_size;
	Vector2 lab_stage_no_off = Vector2(0.2, 0.1) * top_size;
	Vector2 lab_stage_desc_off = Vector2(0.8, 0.1) * bottom_size;
	Vector2 tex_stage_icon_off = Vector2(0.1, 0.0) * bottom_size;
	lab_stage_desc_off.x -= lab_stage_desc->get_size().x;

	lab_stage_name->set_position(lab_stage_name_off);
	lab_stage_no->set_position(lab_stage_no_off);
	lab_stage_desc->set_position(lab_stage_desc_off);
	tex_stage_icon->set_position(tex_stage_icon_off);

	Vector2 base_scale = Vector2(size.y, size.y) / 100.;

	lab_stage_name->set_scale(base_scale * .4);
	lab_stage_no->set_scale(base_scale * .2);
	lab_stage_desc->set_scale(base_scale * .2);
	tex_stage_icon->set_size(Vector2(top_size.y, top_size.y));
}

void UiStageBeginScreen::setup_layout() {
	set_anchors_and_offsets_preset(PRESET_FULL_RECT);

	bgfill = memnew(ColorRect);
	bgfill->set_color(Color::from_hsv(0., 0., 0., 0.2));
	add_child(bgfill);

	top_panel = memnew(Panel);
	add_child(top_panel);

	bottom_panel = memnew(Panel);
	add_child(bottom_panel);

	lab_stage_name = memnew(Label);
	lab_stage_name->set_text(stage_name);
	top_panel->add_child(lab_stage_name);

	lab_stage_no = memnew(Label);
	lab_stage_no->set_text(stage_no);
	top_panel->add_child(lab_stage_no);

	lab_stage_desc = memnew(Label);
	lab_stage_desc->set_text(stage_desc);
	bottom_panel->add_child(lab_stage_desc);

	tex_stage_icon = memnew(TextureRect);
	tex_stage_icon->set_texture(stage_icon);
	tex_stage_icon->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	tex_stage_icon->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	top_panel->add_child(tex_stage_icon);
}

} //namespace godot