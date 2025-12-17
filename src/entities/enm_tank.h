#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

/// @brief A stationary "walking" tank that shoots at player.
class EnmTank : public Enemy {
	GDCLASS(EnmTank, Enemy)

	static constexpr double COLL_R = 1.0;
	static constexpr const char *MDL_PATH = "res://assets/entities/enm_tank/mdl_enm_tank.blend";

	static constexpr int32_t MAX_HP = 20;

	static constexpr double AI_ALERT_RADIUS = 100.0;
	static constexpr double AI_FIRE_RATE = 0.3;

	double t_since_fired = 1.0 / AI_FIRE_RATE;

	Node3D *mdl = nullptr;
	Node3D *mdl_snoot = nullptr;
	CollisionShape3D *coll = nullptr;
	Ref<SphereShape3D> coll_sphere;

protected:
	static void _bind_methods();

public:
	EnmTank();
	~EnmTank() override = default;

	String get_display_name() const override { return "Tonk"; }
	String get_editor_model_path() const override { return "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.obj"; }

	void _ready() override;
	void _process(double delta) override;

	void die() override;

private:
	void setup_model();
	void setup_collider();

	void fire();
};

} //namespace godot