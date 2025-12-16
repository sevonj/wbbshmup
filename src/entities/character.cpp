#include "character.h"

#include <game.h>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

namespace godot {

void Character::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_max_hp"), &Character::get_max_hp);
	ClassDB::bind_method(D_METHOD("set_max_hp"), &Character::set_max_hp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "max_hp"), "set_max_hp", "get_max_hp");
	ClassDB::bind_method(D_METHOD("get_hp"), &Character::get_hp);
	ClassDB::bind_method(D_METHOD("set_hp"), &Character::set_hp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "hp"), "set_hp", "get_hp");

	ClassDB::bind_method(D_METHOD("get_visual_focus_point"), &Character::get_visual_focus_point);
	ClassDB::bind_method(D_METHOD("get_display_name"), &Character::get_display_name);
	ClassDB::bind_method(D_METHOD("get_editor_model_path"), &Character::get_editor_model_path);

	ADD_SIGNAL(MethodInfo("died"));
	ADD_SIGNAL(MethodInfo("damaged"));
}

Character::Character() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		mat_hitflash = ResourceLoader::get_singleton()->load("res://assets/materials/mat_fx_hitflash.tres", "Material");
	}
}

Character::~Character() {}

void Character::add_hp(int32_t value) {
	hp = Math::clamp(hp + value, 0, max_hp);
}

void Character::take_damage(DamageInfo damage) {
	if (invincible) {
		print_line(get_class(), ": iframe dmg cancel");
		return;
	}
	if (hitflash_enabled) {
		trigger_hitflash();
	}
	print_line(get_class(), ": took dmg: '", damage.value, "'");
	hp = Math::clamp(hp - damage.value, 0, max_hp);
	emit_signal("damaged");
	if (hp == 0) {
		die();
	}
}

void Character::die() {
	emit_signal("died");
	print_line(get_class(), ": died");
	queue_free();
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
		if (enable && mat_hitflash.is_valid()) {
			meshinst->set_material_override(mat_hitflash);
		} else {
			meshinst->set_material_override(Ref<Material>());
		}
	}
}
} //namespace godot