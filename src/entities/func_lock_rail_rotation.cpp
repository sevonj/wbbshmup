#include "func_lock_rail_rotation.h"

#include <consts.h>
#include <entities/player.h>
#include <godot_cpp/classes/engine.hpp>

namespace godot {
void FuncLockRailRotation::_bind_methods() {}

void FuncLockRailRotation::_ready() {
	set_collision_layer(COL_LAYER_NONE);
	set_collision_mask(COL_LAYER_PLAYER);
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	connect("body_entered", callable_mp(this, &FuncLockRailRotation::on_body_entered));
	connect("body_exited", callable_mp(this, &FuncLockRailRotation::on_body_exited));
}

void FuncLockRailRotation::on_body_entered(Node3D *body) {
	Player *player = cast_to<Player>(body);
	if (player) {
		player->set_lock_rail_rotation(true);
	}
}

void FuncLockRailRotation::on_body_exited(Node3D *body) {
	Player *player = cast_to<Player>(body);
	if (player) {
		player->set_lock_rail_rotation(false);
	}
}

} //namespace godot