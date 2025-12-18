#pragma once

#include <data/damage_info.h>
#include <entities/projectile.h>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class ProjectilePlayerLaser : public Projectile {
	GDCLASS(ProjectilePlayerLaser, Projectile)

	static constexpr const char *MAT_PATH = "res://assets/materials/mat_fx_playerlaser.tres";

	static constexpr double LIFE_DISTANCE = 120.;
	static constexpr double SPEED = 100.;

protected:
	static void _bind_methods();

public:
	ProjectilePlayerLaser();
	~ProjectilePlayerLaser() override = default;

	void _ready() override;

	void setup_model() override;
};

} //namespace godot