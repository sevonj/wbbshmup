#pragma once

#include <data/damage_info.h>
#include <entities/projectile.h>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Basic bullet for most enemy types.
class ProjectileEnmGeneric : public Projectile {
	GDCLASS(ProjectileEnmGeneric, Projectile)

	static constexpr const char *MAT_PATH = "res://assets/materials/mat_fx_enmlaser.tres";

	static constexpr double LIFE_DISTANCE = 100.0;
	static constexpr double SPEED = 5.0;

protected:
	static void _bind_methods();

public:
	ProjectileEnmGeneric();
	~ProjectileEnmGeneric() override = default;

	void _ready() override;

	void setup_model() override;
};

} //namespace godot