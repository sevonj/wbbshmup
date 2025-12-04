#include "enemy.h"

#include <game.h>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace godot {

void Enemy::_bind_methods() {
}

Enemy::Enemy() {
}

Enemy::~Enemy() {}

void Enemy::_ready() {
	Character::_ready();

	set_collision_layer(COL_LAYER_ENEMIES);
	set_collision_mask(COL_MASK_ENEMIES);
}

} //namespace godot