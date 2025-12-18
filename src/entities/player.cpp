#include "player.h"

#include <assets.h>
#include <config.h>
#include <debug_draw.h>
#include <entities/projectile_player_laser.h>
#include <game.h>
#include <wbb_input.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot {

void Player::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_input_enabled"), &Player::get_input_enabled);
	ClassDB::bind_method(D_METHOD("set_input_enabled"), &Player::set_input_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "input_enabled"), "set_input_enabled", "get_input_enabled");
	ClassDB::bind_method(D_METHOD("get_noclip"), &Player::get_noclip);
	ClassDB::bind_method(D_METHOD("set_noclip"), &Player::set_noclip);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "noclip"), "set_noclip", "get_noclip");
	ClassDB::bind_method(D_METHOD("get_lock_rail_rotation"), &Player::get_lock_rail_rotation);
	ClassDB::bind_method(D_METHOD("set_lock_rail_rotation"), &Player::set_lock_rail_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "lock_rail_rotation"), "set_lock_rail_rotation", "get_lock_rail_rotation");
	ClassDB::bind_method(D_METHOD("get_t_stage_progress"), &Player::get_t_stage_progress);
	ClassDB::bind_method(D_METHOD("set_t_stage_progress"), &Player::set_t_stage_progress);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "t_stage_progress"), "set_t_stage_progress", "get_t_stage_progress");
	ClassDB::bind_method(D_METHOD("get_rail_offset"), &Player::get_rail_offset);
	ClassDB::bind_method(D_METHOD("set_rail_offset"), &Player::set_rail_offset);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "rail_offset"), "set_rail_offset", "get_rail_offset");
}

Player::Player() {
	Game::set_player(this);
}

Player::~Player() {
	Game::set_player(nullptr);
}

void Player::_ready() {
	setup_model();
	setup_collider();
}

void Player::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (config::debug_draw_colliders) {
		DebugDraw::draw_sphere3d(get_global_position(), coll_sphere->get_radius(), COLOR_DEBUG_COLL);
	}

	t_stage_progress += delta;

	WbbInput *input = WbbInput::get_singleton();
	Vector2 input_axis = input->get_axis();

	Vector3 input_dir = Vector3(input_axis.x, 0, input_axis.y);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 orientation = (up + input_dir).normalized();

	if (config::debug_draw_player_input) {
		Transform3D xform = get_global_transform();
		DebugDraw::draw_line_3d(xform.origin, xform.xform(up), Color(1, 0, 0));
		DebugDraw::draw_line_3d(xform.origin, xform.xform(orientation), Color(0, 0, 1));
	}

	Stage *stage = Game::get_stage();
	if (!stage) {
		return;
	}

	const Transform3D old_xform = get_transform();
	const Transform3D rail_xform = stage->sample_rail_at_time(t_stage_progress);
	Transform3D new_xform = rail_xform;
	if (!lock_rail_rotation) {
		rail_basis = rail_xform.basis;
	}
	new_xform.basis = rail_basis;

	float angle = 0.0;
	if (input_enabled) {
		rail_offset += input_dir * delta * LEAN_MOVE_SPEED;
		const Vector3 lean = Vector3(input_axis.y, 0, -input_axis.x) * LEAN_SCALE;
		angle = -input_axis.x * LEAN_TURN_MAX_ANGLE;
		mdl->set_rotation(lean);
	}

	rail_offset.x = Math::clamp(rail_offset.x, -MAX_RAIL_OFF_X, MAX_RAIL_OFF_X);
	rail_offset.z = Math::clamp(rail_offset.z, -MAX_RAIL_OFF_Z, MAX_RAIL_OFF_Z);
	new_xform.translate_local(rail_offset);
	new_xform.basis.rotate(Vector3(0., 1., 0.), angle);

	set_transform(new_xform);
	// Player doesn't use velocity, but some enemies will look at it to predict player movement.
	set_velocity(new_xform.origin - old_xform.origin);
}

void Player::_physics_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	t_since_fired += delta;

	if (!input_enabled) {
		return;
	}

	if (t_since_fired >= 1.0 / FIRE_RATE) {
		fire();
		t_since_fired = 0.0;
	}
}

void Player::setup_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res.is_valid()) {
		mdl = cast_to<Node3D>(mdl_res->instantiate());
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

void Player::setup_collider() {
	set_collision_layer(COL_LAYER_PLAYER);
	set_collision_mask(COL_MASK_PLAYER);

	coll_sphere = (Ref<SphereShape3D>)memnew(SphereShape3D);
	coll_sphere->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(coll_sphere);
	coll->set_name("coll");
	add_child(coll);
}

void Player::fire() {
	ProjectilePlayerLaser *projectile = memnew(ProjectilePlayerLaser);
	Game::get_stage()->add_entity(projectile);
	projectile->set_global_position(get_global_position());
	projectile->set_global_rotation(get_global_rotation());
}

} //namespace godot