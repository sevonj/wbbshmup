#pragma once

#include <data/damage_info.h>

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class BaseCharacter : public CharacterBody3D {
	GDCLASS(BaseCharacter, CharacterBody3D)

private:
protected:
	int32_t max_health;
	int32_t health;
	/// @brief Ignore damage. Use for iframe or godmode.
	bool invincible;

	static void _bind_methods();

public:
	BaseCharacter();
	~BaseCharacter();

	void _ready() override;

	virtual int32_t get_max_health() final;
	virtual void set_max_health(int32_t value) final;
	virtual int32_t get_health() final;
	virtual void set_health(int32_t value) final;
	/// @brief Add or remove health.
	virtual void add_health(int32_t value);
	/// @brief Treated as damage, even if the value is negative.
	virtual void take_damage(DamageInfo damage);
	virtual void die();

	/// @brief Visual center point
	virtual Vector3 get_focus_point();

	virtual String get_display_name();
	virtual String get_editor_model_path();
};

} //namespace godot