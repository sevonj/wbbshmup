#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

/// @brief Rhino is a homing missile that will attempt to ram the player head on. Buff and unwieldy.
class EnmRhino : public Enemy {
	GDCLASS(EnmRhino, Enemy)

	static constexpr double COLL_R = 1.0;
	static constexpr const char *MDL_PATH = "res://assets/entities/enm_rhino/mdl_enm_rhino.blend";

	static constexpr double SPEED = 35.0;
	static constexpr double LIFETIME = 10.0;
	static constexpr int32_t MAX_HP = 100;

	static constexpr double AI_ALERT_RADIUS = 200.;
	static constexpr double AI_NAVGAIN = 3.9;
	static constexpr double AI_TURN_RATE = M_PI * SPEED * 0.01;
	static constexpr double AI_GIVEUP_ANGLE = M_PI * 0.5;

protected:
	static void _bind_methods();

public:
	EnmRhino();
	~EnmRhino();

	void _ready() override;
	void _process(double delta) override;

	void die() override;

	String get_display_name() override { return "Rhino Missile"; }
	String get_editor_model_path() override { return "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.obj"; }

private:
	bool has_launched = false;
	bool has_lost_tgt = false;
	double t_since_launch = 0.0;

	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;
	Ref<SphereShape3D> coll_sphere = Ref<SphereShape3D>();

	void setup_model();
	void setup_collider();
};

} //namespace godot