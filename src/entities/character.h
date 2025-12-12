#pragma once

#include <data/damage_info.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class Character : public CharacterBody3D {
	GDCLASS(Character, CharacterBody3D)

private:
protected:
	static constexpr double HITFLASH_DURATION = 0.1;

	int32_t max_health = 100;
	int32_t health = 100;
	bool invincible = false;
	bool hitflash_enabled = true;

	Ref<Material> mat_hitflash = nullptr;

	static void _bind_methods();

	void trigger_hitflash();
	void set_hitflash(bool enabled);

public:
	Character();
	~Character();

	virtual void _ready() override;

	virtual int32_t get_max_health() final;
	virtual void set_max_health(int32_t value) final;
	virtual int32_t get_health() final;
	virtual void set_health(int32_t value) final;
	virtual void add_health(int32_t value);
	virtual void take_damage(DamageInfo damage);
	virtual void die();

	virtual Vector3 get_focus_point();

	virtual String get_display_name();
	virtual String get_editor_model_path();
};

} //namespace godot