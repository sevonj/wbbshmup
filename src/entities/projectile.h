#pragma once

#include <data/damage_info.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

/// @brief A basic projectile. Works as is for placeholder purposes, but you really should inherit it.
class Projectile : public CharacterBody3D {
	GDCLASS(Projectile, CharacterBody3D)

private:
	static constexpr double COLL_R = 0.1;
	static constexpr double LIFETIME = 10.;
	static constexpr double SPEED = 6.;

protected:
	double lifetimer = LIFETIME;
	double speed = SPEED;

	Node3D *mdl;
	CollisionShape3D *coll;
	Ref<SphereShape3D> coll_sphere = nullptr;

	static void _bind_methods();

public:
	Projectile();
	~Projectile();

	virtual void _ready() override;
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;

	virtual void setup_model();
	virtual void setup_collider();
};

} //namespace godot