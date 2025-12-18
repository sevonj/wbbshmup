#pragma once

#include <consts.h>
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
	static constexpr double LEAN_TURN_MAX_ANGLE = 12.0 * DEG_TO_RAD;
	static constexpr float LEAN_SCALE = 0.4;
	static constexpr float MAX_RAIL_OFF_X = 12.0;
	static constexpr float MAX_RAIL_OFF_Z = 6.0;

	// --- state
	bool input_enabled = false;
	bool noclip = false;
	bool lock_rail_rotation = false;
	double t_stage_progress = 0.0;
	double t_since_fired = 1.0 / FIRE_RATE;

	// --- previous transform
	Vector3 rail_offset;
	Basis rail_basis;

	// --- components
	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;
	Ref<SphereShape3D> coll_sphere;

protected:
	static void _bind_methods();

public:
	Player();
	~Player() override;

	bool get_input_enabled() const { return input_enabled; }
	void set_input_enabled(bool v) { input_enabled = v; }
	bool get_noclip() const { return noclip; }
	void set_noclip(bool v) { noclip = v; }
	bool get_lock_rail_rotation() const { return lock_rail_rotation; }
	void set_lock_rail_rotation(bool v) { lock_rail_rotation = v; }
	double get_t_stage_progress() const { return t_stage_progress; }
	void set_t_stage_progress(double v) { t_stage_progress = v; }
	Vector3 get_rail_offset() const { return rail_offset; }
	void set_rail_offset(Vector3 v) { rail_offset = v; }

	void _ready() override;
	void _process(double delta) override;
	void _physics_process(double delta) override;

private:
	void setup_model();
	void setup_collider();

	void fire();
};

} //namespace godot