#include "enm_tank.h"

#include <assets.h>
#include <entities/player.h>
#include <entities/projectile_enm_generic.h>
#include <game.h>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/variant/typed_array.hpp>

namespace godot {

void EnmTank::_bind_methods() {}

EnmTank::EnmTank() {
	mdl = nullptr;
	mdl_snoot = nullptr;
	coll = nullptr;

	t_since_fired = INFINITY;

	set_max_health(MAX_HP);
}

EnmTank::~EnmTank() {}

void EnmTank::setup_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res != nullptr) {
		mdl = (Node3D *)mdl_res->instantiate();
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);

	mdl_snoot = cast_to<Node3D>(get_node_or_null("mdl/snoot"));
	if (!mdl_snoot) {
		print_line(get_class_static(), "Failed to get mdl_snoot!");
	}
}

void EnmTank::setup_collider() {
	SphereShape3D *sphere = memnew(SphereShape3D);
	sphere->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(sphere);
	coll->set_name("coll");
	add_child(coll);
}

void EnmTank::fire() {
	t_since_fired = 0.;
	ProjectileEnmGeneric *projectile = memnew(ProjectileEnmGeneric);
	Game::get_stage()->add_entity(projectile);
	if (mdl_snoot) {
		projectile->set_global_position(mdl_snoot->get_global_position());
		projectile->set_global_rotation(mdl_snoot->get_global_rotation());
	} else {
		projectile->set_global_position(get_global_position());
		projectile->set_global_rotation(get_global_rotation());
	}
}

void EnmTank::_ready() {
	Enemy::_ready();

	setup_model();
	setup_collider();
}

void EnmTank::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	t_since_fired += delta;

	Player *player = Game::get_player();
	if (!player) {
		return;
	}

	Vector3 pos = get_global_position();
	Vector3 player_pos = player->get_global_position();
	if (pos.distance_to(player_pos) > AI_ALERT_RADIUS) {
		return;
	}

	Vector3 rel_pos = player_pos - pos;
	Vector2 rel_pos2d = Vector2(rel_pos.x, rel_pos.z);
	double angle = -rel_pos2d.angle() - Math::deg_to_rad(90.);
	if (mdl_snoot) {
		mdl_snoot->set_rotation(Vector3(0., angle, 0.));
	}

	if (t_since_fired >= 1. / AI_FIRE_RATE) {
		fire();
	}
}

void EnmTank::die() {
	emit_signal("died");
	print_line(get_class(), " died");
	coll->set_disabled(true);
	get_tree()->create_timer(HITFLASH_DURATION)->connect("timeout", callable_mp((Node *)this, &EnmTank::queue_free));
}

String EnmTank::get_display_name() {
	return "Tonk";
}

String EnmTank::get_editor_model_path() {
	return "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.obj";
}

} //namespace godot