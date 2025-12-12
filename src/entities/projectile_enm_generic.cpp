#include "projectile_enm_generic.h"

#include <game.h>
#include <godot_cpp/classes/csg_sphere3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void ProjectileEnmGeneric::_bind_methods() {
}

ProjectileEnmGeneric::ProjectileEnmGeneric() {
	lifetimer = LIFE_DISTANCE / SPEED;
	speed = SPEED;
}

ProjectileEnmGeneric::~ProjectileEnmGeneric() {}

void ProjectileEnmGeneric::_ready() {
	set_collision_layer(0);
	set_collision_mask(COL_MASK_ENEMIES);

	setup_model();
	setup_collider();
}

void ProjectileEnmGeneric::setup_model() {
	CSGSphere3D *sphere = memnew(CSGSphere3D);
	sphere->set_radius(.2);
	mdl = sphere;
	mdl->set_name("mdl");

	Ref<Material> mat = ResourceLoader::get_singleton()->load(MAT_PATH, "Material");
	if (mat != nullptr) {
		sphere->set_material(mat);
	}

	add_child(mdl);
}

} //namespace godot