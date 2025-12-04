#pragma once

#include <data/damage_info.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

namespace godot {

/// @brief Player pickup-able item. Picked up when touching player.
class Item : public CharacterBody3D {
	GDCLASS(Item, CharacterBody3D)

private:
protected:
	const double LIFETIME = 10.;
	const double SPEED = 6.;

	double lifetimer;

	CollisionShape3D *coll;

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