#include "character.h"

#include <game.h>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

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

void Character::trigger_hitflash() {
	set_hitflash(true);
	get_tree()->create_timer(HITFLASH_DURATION)->connect("timeout", callable_mp(this, &Character::set_hitflash).bind(false));
}

void Character::set_hitflash(bool enable) {
	TypedArray<Node> mdl_nodes = TypedArray<Node>();
	mdl_nodes.append(get_node_or_null("mdl"));

	while (!mdl_nodes.is_empty()) {
		Node *node = cast_to<Node>(mdl_nodes.pop_front());
		if (!node) {
			continue;
		}
		mdl_nodes.append_array(node->get_children());
		MeshInstance3D *meshinst = cast_to<MeshInstance3D>(node);
		if (!meshinst) {
			continue;
		}
		if (enable && mat_hitflash != nullptr) {
			meshinst->set_material_override(mat_hitflash);
		} else {
			meshinst->set_material_override(Ref<Material>());
		}
	}
}

Character::Character() {
	health = 100;
	max_health = 100;
	invincible = false;
	hitflash_enabled = true;

	mat_hitflash = ResourceLoader::get_singleton()->load("res://assets/materials/mat_fx_hitflash.tres", "Material");
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

/// @brief Set health. Not considered damage or healing; shouldn't trigger a reaction.
void Character::set_health(int32_t value) {
	health = Math::clamp(value, 0, max_health);
}

/// @brief Add or remove health. Not considered damage or healing; shouldn't trigger a reaction.
void Character::add_health(int32_t value) {
	health = Math::clamp(health + value, 0, max_health);
}

/// @brief Treated as damage, even if the value is negative and character gains health.
void Character::take_damage(DamageInfo damage) {
	if (invincible) {
		print_line(get_class(), " invincible");
		return;
	}
	if (hitflash_enabled) {
		trigger_hitflash();
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

/// @brief Visual center point
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