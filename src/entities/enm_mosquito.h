#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

/// @brief A stationary "walking" tank that shoots at player.
class EnmMosquito : public Enemy {
	GDCLASS(EnmMosquito, Enemy)

	static constexpr double COLL_R = 1.0;
	static constexpr const char *MDL_PATH = "res://assets/entities/enm_mosquito/mdl_enm_mosquito.blend";
	static constexpr int32_t MAX_HP = 20;
	static constexpr int32_t KILL_SCORE = 10;
	static constexpr double AI_ALERT_RADIUS = 150.0;
	static constexpr float SPEED = 12.0;

	// --- state
	bool running = false;
	double progress = 0.;

	// --- components
	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;
	Ref<SphereShape3D> coll_sphere;

protected:
	static void _bind_methods();

public:
	EnmMosquito();
	~EnmMosquito() override = default;

	String get_display_name() const override { return "Mosquito"; }
	String get_editor_model_path() const override { return "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.obj"; }

	void _ready() override;
	void _process(double delta) override;

	void die() override;

private:
	void setup_model();
	void setup_collider();
	Ref<Curve3D> get_curve();
};

} //namespace godot