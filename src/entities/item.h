#pragma once

#include <consts.h>
#include <data/damage_info.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

/// @brief Player pickup-able item. Picked up when touching player.
class Item : public CharacterBody3D {
	GDCLASS(Item, CharacterBody3D)

	static constexpr const char *DEFAULT_MDL_PATH = "res://assets/models/mdl_demo_cube.blend";
	static constexpr double COLL_R = 0.3;
	static constexpr double ROT_SPEED = 120.0 * DEG_TO_RAD;

protected:
	// --- state
	double lifetime = 0.0;

	// --- components
	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;
	Ref<SphereShape3D> coll_sphere;

	static void _bind_methods();

public:
	Item() = default;
	~Item() override = default;

	virtual String get_display_name() const { return "Base Item"; }
	virtual String get_pickup_message() const { return vformat("Picked up %s", get_display_name()); }

	virtual void _ready() override;
	virtual void _process(double delta) override;
	virtual void _physics_process(double delta) override;

	virtual void pickup();

private:
	virtual void setup_collider();
	virtual void setup_model();
};

} //namespace godot