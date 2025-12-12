#pragma once

#include <data/damage_info.h>
#include <entities/projectile.h>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Basic bullet for most enemy types.
class ProjectileEnmGeneric : public Projectile {
	GDCLASS(ProjectileEnmGeneric, Projectile)

private:
	static constexpr double LIFE_DISTANCE = 100.;
	static constexpr double SPEED = 5.;

	const String MAT_PATH = "res://assets/materials/mat_fx_enmlaser.tres";

	double lifetimer = LIFE_DISTANCE;

	Node3D *mdl;
	CollisionShape3D *coll;

	void setup_model();
	void setup_collider();

protected:
	static void _bind_methods();

public:
	ProjectileEnmGeneric();
	~ProjectileEnmGeneric();

	void _ready() override;
	void _physics_process(double delta) override;
};

} //namespace godot