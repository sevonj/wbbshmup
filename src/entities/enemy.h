#pragma once

#include <data/damage_info.h>
#include <entities/character.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class Enemy : public Character {
	GDCLASS(Enemy, Character)

private:
protected:
	static void _bind_methods();

public:
	Enemy();
	~Enemy();

	virtual void _ready() override;
};

} //namespace godot