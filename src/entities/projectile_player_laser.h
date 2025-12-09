#pragma once

#include <data/damage_info.h>
#include <entities/projectile.h>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class ProjectilePlayerLaser : public Projectile {
	GDCLASS(ProjectilePlayerLaser, Projectile)

private:
	static constexpr double LIFETIME = 10.;
	static constexpr double SPEED = 80.;

	const String MAT_PATH = "res://assets/materials/mat_fx_playerlaser.tres";

	double lifetimer;

	Node3D *mdl;
	CollisionShape3D *coll;

	void setup_model();
	void setup_collider();

protected:
	static void _bind_methods();

public:
	ProjectilePlayerLaser();
	~ProjectilePlayerLaser();

	void _ready() override;
	void _physics_process(double delta) override;
};

} //namespace godot