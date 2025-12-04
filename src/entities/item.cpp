#include "item.h"

#include <game.h>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void Item::_bind_methods() {
	ClassDB::bind_method(D_METHOD("pickup"), &Item::pickup);
}

Item::Item() {
	lifetimer = LIFETIME;
}

Item::~Item() {}

void Item::_ready() {
	set_collision_layer(0);
	set_collision_mask(COL_MASK_ITEMS);

	setup_collider();
}

void Item::_process(double delta) {
}

void Item::_physics_process(double delta) {
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
		Player *playa = cast_to<Player>(collision->get_collider());
		if (playa) {
			pickup();
			queue_free();
		}
	}
}

void Item::setup_collider() {
	SphereShape3D *sphere = memnew(SphereShape3D);
	sphere->set_radius(.1);
	coll = memnew(CollisionShape3D);
	coll->set_shape(sphere);
	coll->set_name("coll");
	add_child(coll);
}

void Item::pickup() {
	print_line("Picked up an item!");
}

} //namespace godot