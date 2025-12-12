#include "projectile.h"

#include <entities/character.h>
#include <game.h>
#include <godot_cpp/classes/csg_sphere3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void Projectile::_bind_methods() {
}

Projectile::Projectile() {
}

Projectile::~Projectile() {}

void Projectile::_ready() {
	set_collision_layer(0);
	set_collision_mask(COL_MASK_ENEMIES);

	setup_model();
	setup_collider();
}

void Projectile::_physics_process(double delta) {
	lifetimer -= delta;

	if (Game::is_out_of_bounds(this) || lifetimer <= 0) {
		queue_free();
		return;
	}

	Vector3 direction = -get_global_basis().get_column(2).normalized();
	set_velocity(direction * SPEED);

	Vector3 velocity = get_velocity();
	Ref<KinematicCollision3D> collision = move_and_collide(velocity * delta);
	if (collision.is_valid()) {
		DamageInfo dmg = DamageInfo(10, collision->get_normal() * 2.);

		Node3D *body = cast_to<Node3D>(collision->get_collider());
		if (body) {
			Vector3 knockback = (body->get_global_position() - get_global_position()).normalized();

			Character *basechar = cast_to<Character>(body);
			if (basechar) {
				basechar->take_damage(dmg);
			}

			queue_free();
		}
	}
}

void Projectile::setup_model() {
	CSGSphere3D *sphere = memnew(CSGSphere3D);
	sphere->set_radius(.1);
	mdl = sphere;
	mdl->set_name("mdl");
	add_child(mdl);
}

void Projectile::setup_collider() {
	SphereShape3D *sphere = memnew(SphereShape3D);
	sphere->set_radius(.1);
	coll = memnew(CollisionShape3D);
	coll->set_shape(sphere);
	coll->set_name("coll");
	add_child(coll);
}

} //namespace godot