#pragma once

#include <data/damage_info.h>
#include <entities/projectile.h>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class ProjectilePlayerLaser : public Projectile {
	GDCLASS(ProjectilePlayerLaser, Projectile)

private:
	const double LIFETIME = 10.;
	const double SPEED = 80.;

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