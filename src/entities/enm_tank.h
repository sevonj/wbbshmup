#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief A stationary "walking" tank that shoots at player.
class EnmTank : public Enemy {
	GDCLASS(EnmTank, Enemy)

private:
	static constexpr double COLL_R = 1.0;
	static constexpr int32_t MAX_HP = 20;
	static constexpr double AI_ALERT_RADIUS = 100.;
	static constexpr double AI_FIRE_RATE = 0.3;

	const String MDL_PATH = "res://assets/entities/enm_tank/mdl_enm_tank.blend";

	Node3D *mdl = nullptr;
	Node3D *mdl_snoot = nullptr;
	CollisionShape3D *coll = nullptr;

	double t_since_fired = INFINITY;

	void setup_model();
	void setup_collider();

	void fire();

protected:
	static void _bind_methods();

public:
	EnmTank();
	~EnmTank();

	void _ready() override;
	void _process(double delta) override;

	void die() override;

	String get_display_name() override;
	String get_editor_model_path() override;
};

} //namespace godot