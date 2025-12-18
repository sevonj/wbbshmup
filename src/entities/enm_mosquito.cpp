#include "enm_mosquito.h"

#include <assets.h>
#include <config.h>
#include <consts.h>
#include <debug_draw.h>
#include <entities/player.h>
#include <game.h>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/path3d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void EnmMosquito::_bind_methods() {}

EnmMosquito::EnmMosquito() {
	set_max_hp(MAX_HP);
}

void EnmMosquito::_ready() {
	Enemy::_ready();

	setup_model();
	setup_collider();
}

void EnmMosquito::_process(double delta) {
	Ref<Curve3D> curve = get_curve();
	if (!curve.is_valid() || curve->get_point_count() < 2) {
		set_position(Vector3());
		return;
	}

	if (Engine::get_singleton()->is_editor_hint()) {
		set_transform(curve->sample_baked_with_rotation(0, false, true));
		return;
	}

	if (config::debug_draw_colliders) {
		DebugDraw::draw_sphere3d(get_global_position(), coll_sphere->get_radius(), COLOR_DEBUG_COLL);
	}

	Player *player = Game::get_player();
	if (!player || !player->get_input_enabled()) {
		return;
	}

	Vector3 pos = get_global_position();
	Vector3 player_pos = player->get_global_position();
	if (pos.distance_to(player_pos) <= AI_ALERT_RADIUS) {
		if (!running) {
			print_line("Mosquito: Alerted");
		}
		running = true;
	}

	float length = curve->get_baked_length();
	if (running) {
		progress += delta * SPEED;
	}
	set_transform(curve->sample_baked_with_rotation(progress, false, true));

	if (progress > length) {
		queue_free();
	}
}

void EnmMosquito::die() {
	emit_signal("died");
	print_line(get_class(), " died");
	coll->set_disabled(true);
	get_tree()->create_timer(HITFLASH_DURATION)->connect("timeout", callable_mp((Node *)this, &EnmMosquito::queue_free));
	Game::get_stage()->add_score(KILL_SCORE);
}

void EnmMosquito::setup_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res.is_valid()) {
		mdl = cast_to<Node3D>(mdl_res->instantiate());
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

void EnmMosquito::setup_collider() {
	coll_sphere = (Ref<SphereShape3D>)memnew(SphereShape3D);
	coll_sphere->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(coll_sphere);
	coll->set_name("coll");
	add_child(coll);
}

Ref<Curve3D> EnmMosquito::get_curve() {
	Path3D *parent = cast_to<Path3D>(get_parent());
	if (parent) {
		return parent->get_curve();
	}
	return Ref<Curve3D>();
}
} //namespace godot