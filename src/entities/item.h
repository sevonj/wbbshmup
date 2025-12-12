#pragma once

#include <data/damage_info.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

/// @brief Player pickup-able item. Picked up when touching player.
class Item : public CharacterBody3D {
	GDCLASS(Item, CharacterBody3D)

private:
	static constexpr double COLL_R = 0.1;
	static constexpr double LIFETIME = 10.;
	static constexpr double SPEED = 6.;

protected:
	double lifetimer = LIFETIME;

	CollisionShape3D *coll;
	Ref<SphereShape3D> coll_sphere = nullptr;

	static void _bind_methods();

public:
	Item();
	~Item();

	virtual void _ready() override;
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;
	virtual void setup_collider();
	virtual void pickup();
};

} //namespace godot