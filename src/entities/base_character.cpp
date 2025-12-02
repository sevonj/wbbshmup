#include "base_character.h"

#include <game.h>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace godot {

void BaseCharacter::_bind_methods() {
	ADD_SIGNAL(MethodInfo("died"));
	ADD_SIGNAL(MethodInfo("damaged"));

	ClassDB::bind_method(D_METHOD("get_max_health"), &BaseCharacter::get_max_health);
	ClassDB::bind_method(D_METHOD("set_max_health"), &BaseCharacter::set_max_health);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_health", PROPERTY_HINT_NONE, "Max health"), "set_max_health", "get_max_health");

	ClassDB::bind_method(D_METHOD("get_health"), &BaseCharacter::get_health);
	ClassDB::bind_method(D_METHOD("set_health"), &BaseCharacter::set_health);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "health", PROPERTY_HINT_NONE, "BaseCharacter health"), "set_health", "get_health");

	godot::ClassDB::bind_method(godot::D_METHOD("get_editor_model_path"), &BaseCharacter::get_editor_model_path);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "editor_model_path"), "", "get_editor_model_path");
}

BaseCharacter::BaseCharacter() {
	health = 100;
	max_health = 100;
	invincible = false;
}

BaseCharacter::~BaseCharacter() {}

void BaseCharacter::_ready() {
}

int32_t BaseCharacter::get_max_health() {
	return max_health;
}

void BaseCharacter::set_max_health(int32_t value) {
	max_health = value;
	health = Math::clamp(health, 0, max_health);
}

int32_t BaseCharacter::get_health() {
	return health;
}

void BaseCharacter::set_health(int32_t value) {
	health = Math::clamp(value, 0, max_health);
}

void BaseCharacter::add_health(int32_t value) {
	health = Math::clamp(health + value, 0, max_health);
}

void BaseCharacter::take_damage(DamageInfo damage) {
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

void BaseCharacter::die() {
	emit_signal("died");
	print_line(get_class(), " died");
	queue_free();
}

Vector3 BaseCharacter::get_focus_point() {
	return get_global_position();
}

String BaseCharacter::get_display_name() {
	return get_class();
}

String BaseCharacter::get_editor_model_path() {
	return "res://assets/models/mdl_debug_error.obj";
}

} //namespace godot