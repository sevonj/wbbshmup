#pragma once

#include <entities/character.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot {

class Player : public Character {
	GDCLASS(Player, Character)

	static constexpr double COLL_R = 1.5;
	static constexpr const char *MDL_PATH = "res://assets/entities/player/mdl_player_butterfly.blend";

	static constexpr double FIRE_RATE = 3.0;
	static constexpr double LEAN_MOVE_SPEED = 12.0;
	/// @brief How much does the model rotate towards the movement direction
	static constexpr float MDL_LEAN_SCALE = 0.4;
	/// @brief Maximum position offset X
	static constexpr float MAX_X = 12.0;
	/// @brief Maximum position offset Z
	static constexpr float MAX_Z = 6.0;

	bool enabled = false;
	bool noclip = false;

	double t_since_fired = 1.0 / FIRE_RATE;
	/// @brief Rail follower velocity. Added to actual velocity so enemies can track player movement.
	Vector3 rail_vel;

	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;
	Ref<SphereShape3D> coll_sphere = Ref<SphereShape3D>();

protected:
	static void _bind_methods();

public:
	Player();
	~Player() override;

	void set_rail_vel(Vector3 value) { rail_vel = value; }
	bool get_enabled() const { return enabled; }
	void set_enabled(bool value) { enabled = value; }
	bool get_noclip() const { return noclip; }
	void set_noclip(bool value) { noclip = value; }

	void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

private:
	void setup_model();
	void setup_collider();

	void fire();
};

} //namespace godot