#pragma once

#include <entities/character.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/wrapped.hpp>
#include <godot_cpp/variant/variant.hpp>

using namespace godot;

class Player : public Character {
	GDCLASS(Player, Character)

private:
	static constexpr float COLL_H = 1.7;
	static constexpr float COLL_R = 0.3;
	const double FIRE_RATE = 3.0;

	const String MDL_PATH = "res://assets/entities/player/mdl_player_ship.blend";

	/// @brief How much does the model rotate towards the movement direction
	static constexpr float MDL_LEAN_SCALE = 0.4;
	/// @brief Maximum position offset X
	static constexpr float MAX_X = 12.0;
	/// @brief Maximum position offset Z
	static constexpr float MAX_Z = 6.0;

	Node3D *mdl;
	CollisionShape3D *coll;

	double fire_timer;

	bool enabled;
	bool noclip;

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

	bool get_enabled();
	void set_enabled(bool value);
	bool get_noclip();
	void set_noclip(bool value);
};
