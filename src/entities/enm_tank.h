#pragma once

#include <entities/enemy.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class EnmTank : public Enemy {
	GDCLASS(EnmTank, Enemy)

private:
	static constexpr float SPEED = 0.5;
	static constexpr float COLL_R = 1.;
	static constexpr int32_t MAX_HP = 30;

	const String MDL_PATH = "res://assets/characters/enm_tank/mdl_tank.blend";

	Node3D *mdl;
	CollisionShape3D *coll;

	void setup_model();
	void setup_collider();

protected:
	static void _bind_methods();

public:
	EnmTank();
	~EnmTank();

	void _ready() override;
	void _process(double delta) override;

	String get_display_name() override;
	String get_editor_model_path() override;
};

} //namespace godot