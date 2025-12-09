#include "enm_tank.h"

#include <assets.h>
#include <game.h>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void EnmTank::_bind_methods() {}

EnmTank::EnmTank() {
	mdl = nullptr;
	coll = nullptr;

	set_max_health(MAX_HP);
}

EnmTank::~EnmTank() {}

void EnmTank::setup_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res != nullptr) {
		mdl = (Node3D *)mdl_res->instantiate();
	} else {
		mdl = Assets::instance_error_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

void EnmTank::setup_collider() {
	SphereShape3D *sphere = memnew(SphereShape3D);
	sphere->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(sphere);
	coll->set_name("coll");
	add_child(coll);
}

void EnmTank::_ready() {
	Enemy::_ready();

	setup_model();
	setup_collider();
}

void EnmTank::_process(double delta) {
}

String EnmTank::get_display_name() {
	return "Tank";
}

String EnmTank::get_editor_model_path() {
	return "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.obj";
}

} //namespace godot