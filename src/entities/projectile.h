#pragma once

#include <data/damage_info.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief A basic projectile. Works as is for placeholder purposes, but you really should inherit it.
class Projectile : public CharacterBody3D {
	GDCLASS(Projectile, CharacterBody3D)

private:
	const double LIFETIME = 10.;
	const double SPEED = 6.;

	double lifetimer;

	Node3D *mdl;
	CollisionShape3D *coll;

	void setup_model();
	void setup_collider();

protected:
	static void _bind_methods();

public:
	Projectile();
	~Projectile();

	virtual void _ready() override;
	virtual void _physics_process(double delta) override;
};

} //namespace godot