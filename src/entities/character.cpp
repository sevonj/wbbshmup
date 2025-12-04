#include "character.h"

#include <game.h>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace godot {

void Character::_bind_methods() {
	ADD_SIGNAL(MethodInfo("died"));
	ADD_SIGNAL(MethodInfo("damaged"));

	ClassDB::bind_method(D_METHOD("get_max_health"), &Character::get_max_health);
	ClassDB::bind_method(D_METHOD("set_max_health"), &Character::set_max_health);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health", PROPERTY_HINT_NONE, "Max health"), "set_max_health", "get_max_health");

	ClassDB::bind_method(D_METHOD("get_health"), &Character::get_health);
	ClassDB::bind_method(D_METHOD("set_health"), &Character::set_health);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "health", PROPERTY_HINT_NONE, "Character health"), "set_health", "get_health");

	godot::ClassDB::bind_method(godot::D_METHOD("get_editor_model_path"), &Character::get_editor_model_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "editor_model_path"), "", "get_editor_model_path");
}

Character::Character() {
	health = 100;
	max_health = 100;
	invincible = false;
}

Character::~Character() {}

void Character::_ready() {
}

int32_t Character::get_max_health() {
	return max_health;
}

void Character::set_max_health(int32_t value) {
	max_health = value;
	health = Math::clamp(health, 0, max_health);
}

int32_t Character::get_health() {
	return health;
}

void Character::set_health(int32_t value) {
	health = Math::clamp(value, 0, max_health);
}

void Character::add_health(int32_t value) {
	health = Math::clamp(health + value, 0, max_health);
}

void Character::take_damage(DamageInfo damage) {
	if (invincible) {
		print_line(get_class(), " invincible");
		return;
	}
	print_line(get_class(), " took dmg: ", damage.value);
	health = Math::clamp(health - damage.value, 0, max_health);
	emit_signal("damaged");
	if (health == 0) {
		die();
	}
}

void Character::die() {
	emit_signal("died");
	print_line(get_class(), " died");
	queue_free();
}

Vector3 Character::get_focus_point() {
	return get_global_position();
}

String Character::get_display_name() {
	return get_class();
}

String Character::get_editor_model_path() {
	return "res://assets/models/mdl_debug_error.obj";
}

} //namespace godot