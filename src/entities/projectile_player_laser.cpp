#include "projectile_player_laser.h"

#include <game.h>
#include <godot_cpp/classes/csg_sphere3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void ProjectilePlayerLaser::_bind_methods() {
}

ProjectilePlayerLaser::ProjectilePlayerLaser() {
	lifetimer = LIFE_DISTANCE / SPEED;
	speed = SPEED;
}

ProjectilePlayerLaser::~ProjectilePlayerLaser() {}

void ProjectilePlayerLaser::_ready() {
	print_line("createdd");
	set_collision_mask(COL_MASK_PLAYER_PROJECTILE);

	setup_model();
	setup_collider();
}

void ProjectilePlayerLaser::setup_model() {
	CSGSphere3D *sphere = memnew(CSGSphere3D);
	sphere->set_radius(.1);
	mdl = sphere;
	mdl->set_name("mdl");

	Ref<Material> mat = ResourceLoader::get_singleton()->load(MAT_PATH, "Material");
	if (mat != nullptr) {
		sphere->set_material(mat);
	}

	add_child(mdl);
}

} //namespace godot