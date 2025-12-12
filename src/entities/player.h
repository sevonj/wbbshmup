#pragma once

#include <entities/character.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class Player : public Character {
	GDCLASS(Player, Character)

private:
	static constexpr double COLL_R = 1.5;
	static constexpr double FIRE_RATE = 3.0;

	const String MDL_PATH = "res://assets/entities/player/mdl_player_butterfly.blend";

	/// @brief How much does the model rotate towards the movement direction
	static constexpr float MDL_LEAN_SCALE = 0.4;
	/// @brief Maximum position offset X
	static constexpr float MAX_X = 12.0;
	/// @brief Maximum position offset Z
	static constexpr float MAX_Z = 6.0;

	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;
	Ref<SphereShape3D> coll_sphere = nullptr;

	double fire_timer = 0.;
	/// @brief On-rail velocity. Not used by this class, but added to velocity so enemies can track player velocity.
	Vector3 rail_vel = Vector3();

	bool enabled = false;
	bool noclip = false;

	void setup_model();
	void setup_collider();
	void fire();

protected:
	static void _bind_methods();

public:
	Player();
	~Player();

	void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

	void set_rail_vel(Vector3 value);
	bool get_enabled();
	void set_enabled(bool value);
	bool get_noclip();
	void set_noclip(bool value);
};
