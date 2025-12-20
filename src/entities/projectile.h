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

	static constexpr double COLL_R = 0.1;
	static constexpr double DEFAULT_LIFETIME = 10.0;
	static constexpr double DEFAULT_SPEED = 6.0;
	static constexpr int32_t DEFAULT_DAMAGE = 10;

protected:
	static void _bind_methods();

public:
	Projectile() = default;
	~Projectile() override = default;

	virtual void _ready() override;
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;

	virtual void setup_model();
	virtual void setup_collider();

protected:
	// --- state, config
	double lifetimer = DEFAULT_LIFETIME;
	double speed = DEFAULT_SPEED;
	int32_t damage = DEFAULT_DAMAGE;

	// --- components
	Node3D *mdl;
	CollisionShape3D *coll;
	Ref<SphereShape3D> coll_sphere;
};

} //namespace godot