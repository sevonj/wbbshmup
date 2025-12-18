#include "func_trigger.h"

#include <consts.h>
#include <entities/player.h>
#include <godot_cpp/classes/engine.hpp>

namespace godot {
void FuncTrigger::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_enabled"), &FuncTrigger::set_enabled);
	ClassDB::bind_method(D_METHOD("get_enabled"), &FuncTrigger::get_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled"), "set_enabled", "get_enabled");
	ClassDB::bind_method(D_METHOD("set_trigger_on_enter"), &FuncTrigger::set_trigger_on_enter);
	ClassDB::bind_method(D_METHOD("get_trigger_on_enter"), &FuncTrigger::get_trigger_on_enter);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "trigger_on_enter"), "set_trigger_on_enter", "get_trigger_on_enter");
	ClassDB::bind_method(D_METHOD("set_trigger_on_exit"), &FuncTrigger::set_trigger_on_exit);
	ClassDB::bind_method(D_METHOD("get_trigger_on_exit"), &FuncTrigger::get_trigger_on_exit);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "trigger_on_exit"), "set_trigger_on_exit", "get_trigger_on_exit");
	ClassDB::bind_method(D_METHOD("set_oneshot"), &FuncTrigger::set_oneshot);
	ClassDB::bind_method(D_METHOD("get_oneshot"), &FuncTrigger::get_oneshot);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "oneshot"), "set_oneshot", "get_oneshot");

	ADD_SIGNAL(MethodInfo("triggered"));
}

void FuncTrigger::_ready() {
	set_collision_layer(COL_LAYER_NONE);
	set_collision_mask(COL_LAYER_PLAYER);
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	connect("body_entered", callable_mp(this, &FuncTrigger::on_body_entered));
	connect("body_exited", callable_mp(this, &FuncTrigger::on_body_exited));
}

void FuncTrigger::on_body_entered(Node3D *body) {
	Player *player = cast_to<Player>(body);
	if (enabled && trigger_on_enter && player) {
		if (oneshot) {
			enabled = false;
		}
		emit_signal("triggered");
	}
}

void FuncTrigger::on_body_exited(Node3D *body) {
	Player *player = cast_to<Player>(body);
	if (enabled && trigger_on_exit && player) {
		if (oneshot) {
			enabled = false;
		}
		emit_signal("triggered");
	}
}

} //namespace godot