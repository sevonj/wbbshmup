#include "ui_hud_rhino_alert.h"

#include <entities/camera_rig.h>
#include <game.h>
#include <singleton/debug_draw.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/viewport.hpp>

namespace godot {

void UiHudRhinoAlert::_bind_methods() {
}

UiHudRhinoAlert::UiHudRhinoAlert() {
}

UiHudRhinoAlert::~UiHudRhinoAlert() {
}

void UiHudRhinoAlert::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	setup_layout();
}

void UiHudRhinoAlert::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (!target) {
		return;
	}

	Vector3 tgt_pos = target->get_global_position();

	Viewport *vp = get_viewport();
	Rect2 rect = vp->get_visible_rect();

	CameraRig *camera_rig = Game::get_current_camera();
	if (!camera_rig) {
		return;
	}
	Camera3D *camera = camera_rig->get_camera();
	if (camera->is_position_behind(tgt_pos)) {
		hide();
		return;
	}

	Vector2 center = rect.get_center();
	Vector2 scr_pos = camera->unproject_position(tgt_pos);
	if (rect.has_point(scr_pos)) {
		// Rhino is visible. Show lock on graphic.
		// TODO: replace hide+debugdraw with a proper lock-on texture
		hide();
		DebugDraw::draw_point_2d(scr_pos, Color::from_hsv(1., 1., 1., 1.));
	} else {
		// Rhino is off-screen. Show arrow that points towards it.
		show();
		Vector2 scr_dir = (scr_pos - center).normalized();
		scr_pos = center + scr_dir * rect.size.y * .8 / 2.;
	}

	set_global_position(scr_pos - get_size() / 2.);
	set_rotation((scr_pos - center).angle() + Math::deg_to_rad(90.));
}

void UiHudRhinoAlert::set_target(EnmRhino *value) {
	target = value;
	target->connect("tree_exiting", callable_mp((Node *)this, &Node::queue_free));
}

void UiHudRhinoAlert::setup_layout() {
	Ref<Texture2D> tex = godot::ResourceLoader::get_singleton()->load(TEX_PATH, "ImageTexture");
	if (tex != nullptr) {
		set_texture(tex);
	}
}

} //namespace godot