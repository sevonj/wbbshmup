#include "item_energy.h"

#include <game.h>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void ItemEnergy::_bind_methods() {
	ClassDB::bind_method(D_METHOD("pickup"), &ItemEnergy::pickup);
}

ItemEnergy::ItemEnergy() {
	lifetimer = LIFETIME;
}

ItemEnergy::~ItemEnergy() {}

void ItemEnergy::_ready() {
	Item::_ready();
}

void ItemEnergy::_process(double delta) {
}

/*
void ItemEnergy::_physics_process(double delta) {
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
}*/

void ItemEnergy::pickup() {
	print_line("Picked up an ItemEnergy!");
}

} //namespace godot