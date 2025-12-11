#include "player.h"

#include <assets.h>
#include <entities/projectile_player_laser.h>
#include <game.h>
#include <singleton/debug_draw.h>
#include <singleton/wbb_input.h>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/variant.hpp>

void Player::setup_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res != nullptr) {
		mdl = (Node3D *)mdl_res->instantiate();
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

void Player::setup_collider() {
	set_collision_layer(COL_LAYER_PLAYER);
	set_collision_mask(COL_MASK_PLAYER);

	CapsuleShape3D *capsule = memnew(CapsuleShape3D);
	capsule->set_height(COLL_H);
	capsule->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(capsule);
	coll->set_name("coll");
	add_child(coll);
	coll->set_position(Vector3(0, COLL_H / 2.0, 0));
}

void Player::fire() {
	ProjectilePlayerLaser *projectile = memnew(ProjectilePlayerLaser);
	Game::get_stage()->add_entity(projectile);
	projectile->set_global_position(get_global_position());
	projectile->set_global_rotation(get_global_rotation());
}

void Player::_bind_methods() {
}

Player::Player() {
	Game::set_player(this);
}

Player::~Player() {
}

void Player::_ready() {
	setup_model();
	setup_collider();
}

void Player::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	WbbInput *input = WbbInput::get_singleton();
	Vector2 input_axis = input->get_axis();

	Vector3 input_dir = Vector3(input_axis.x, 0, input_axis.y);
	Vector3 up = Vector3(0, 1, 0);
	Vector3 orientation = (up + input_dir).normalized();

	Transform3D xform = get_global_transform();
	DebugDraw::draw_line_3d(xform.origin, xform.xform(up), Color(1, 0, 0));
	DebugDraw::draw_line_3d(xform.origin, xform.xform(orientation), Color(0, 0, 1));

	if (!enabled) {
		return;
	}

	mdl->set_rotation(Vector3(Vector3(input_axis.y, 0, -input_axis.x) * MDL_LEAN_SCALE));

	Vector3 pos = get_position();
	Vector3 old_pos = pos;

	pos += input_dir * delta * 12.;
	if (pos.x > MAX_X) {
		pos.x = MAX_X;
	} else if (pos.x < -MAX_X) {
		pos.x = -MAX_X;
	}
	if (pos.z > MAX_Z) {
		pos.z = MAX_Z;
	} else if (pos.z < -MAX_Z) {
		pos.z = -MAX_Z;
	}
	set_position(pos);

	// We don't actually use velocity here, but some enemies do to predict player movement.
	set_velocity(pos - old_pos);
}

void Player::_physics_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	fire_timer -= delta * FIRE_RATE;

	if (!enabled) {
		return;
	}

	if (fire_timer <= 0) {
		fire();
		fire_timer = 1;
	}
}

void Player::set_rail_vel(Vector3 value) {
	rail_vel = value;
}

bool Player::get_enabled() {
	return enabled;
}

void Player::set_enabled(bool value) {
	enabled = value;
}

bool Player::get_noclip() {
	return noclip;
}

void Player::set_noclip(bool value) {
	noclip = value;
}
