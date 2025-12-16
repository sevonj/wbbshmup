#pragma once

#include <data/damage_info.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Base class that provides health and other character related functionality.
class Character : public CharacterBody3D {
	GDCLASS(Character, CharacterBody3D)

	static constexpr int32_t DEFAULT_HP = 100;

protected:
	static constexpr double HITFLASH_DURATION = 0.1;

	int32_t max_hp = DEFAULT_HP;
	int32_t hp = DEFAULT_HP;
	bool invincible = false;
	bool hitflash_enabled = true;

	Ref<Material> mat_hitflash = Ref<Material>();

	static void _bind_methods();

public:
	Character();
	~Character() override;

	void set_max_hp(int32_t value) {
		max_hp = value;
		hp = Math::clamp(hp, 0, max_hp);
	}
	int32_t get_max_hp() const {
		return max_hp;
	}
	void set_hp(int32_t value) {
		hp = Math::clamp(value, 0, max_hp);
	}
	int32_t get_hp() const {
		return hp;
	}

	virtual void add_hp(int32_t value);
	virtual void take_damage(DamageInfo damage);
	virtual void die();

	virtual Vector3 get_visual_focus_point() const { return get_global_position(); }
	virtual String get_display_name() const { return get_class(); }
	virtual String get_editor_model_path() const { return "res://assets/models/mdl_debug_error.obj"; }

protected:
	void trigger_hitflash();
	void set_hitflash(bool enabled);
};

} //namespace godot