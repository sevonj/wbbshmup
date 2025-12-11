#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Part cock. Part lobster. The future of bossfights.
class EnmBossCocklobster : public Enemy {
	GDCLASS(EnmBossCocklobster, Enemy)

private:
	const float speed = 0.5;
	static constexpr float COLL_H = 1.7;
	static constexpr float COLL_R = 0.3;

	const String MDL_PATH = "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.blend";

	Node3D *mdl;
	CollisionShape3D *coll;

	void setup_model();
	void setup_collider();

protected:
	static void _bind_methods();

public:
	EnmBossCocklobster();
	~EnmBossCocklobster();

	void _ready() override;
	void create_bossbar();
	void _process(double delta) override;

	String get_display_name() override;
	String get_editor_model_path() override;
};

} //namespace godot