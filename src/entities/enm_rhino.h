#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Rhino is a homing missile that will attempt to ram the player head on. Buff and unwieldy.
class EnmRhino : public Enemy {
	GDCLASS(EnmRhino, Enemy)

private:
	static constexpr double SPEED = 35.;
	static constexpr double COLL_R = 1.0;
	static constexpr int32_t MAX_HP = 100;
	static constexpr double AI_ALERT_RADIUS = 200.;
	static constexpr double AI_TURN_RATE = M_PI * SPEED * 0.1;
	static constexpr double AI_GIVEUP_ANGLE = M_PI;
	static constexpr double LIFETIME = 10.;

	const String MDL_PATH = "res://assets/entities/enm_rhino/mdl_enm_rhino.blend";

	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;

	bool launched = false;
	bool gave_up = false;
	double t_since_launch = 0.;

	void setup_model();
	void setup_collider();

protected:
	static void _bind_methods();

public:
	EnmRhino();
	~EnmRhino();

	void _ready() override;
	void _process(double delta) override;

	void die() override;

	String get_display_name() override;
	String get_editor_model_path() override;
};

} //namespace godot