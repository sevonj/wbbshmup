#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Part cock. Part lobster. The future of bossfights.
class EnmBossClobster : public Enemy {
	GDCLASS(EnmBossClobster, Enemy)

	static constexpr float COLL_H = 1.7;
	static constexpr float COLL_R = 0.3;
	static constexpr const char *MDL_PATH = "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.blend";

	static constexpr float SPEED = 0.5;

	Node3D *mdl = nullptr;
	CollisionShape3D *coll = nullptr;

protected:
	static void _bind_methods();

public:
	EnmBossClobster() = default;
	~EnmBossClobster() override = default;

	String get_display_name() const override { return "Cock Lobster"; }
	String get_editor_model_path() const override { return "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.obj"; }

	void _ready() override;
	void _process(double delta) override;

	void create_bossbar();

private:
	void setup_model();
	void setup_collider();
};

} //namespace godot