#include "func_mesh_mover.h"

#include <godot_cpp/classes/engine.hpp>

namespace godot {

void FuncMeshMover::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_trigger"), &FuncMeshMover::set_trigger);
	ClassDB::bind_method(D_METHOD("get_trigger"), &FuncMeshMover::get_trigger);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trigger", PROPERTY_HINT_NODE_TYPE), "set_trigger", "get_trigger");
	ClassDB::bind_method(D_METHOD("set_end_offset"), &FuncMeshMover::set_end_offset);
	ClassDB::bind_method(D_METHOD("get_end_offset"), &FuncMeshMover::get_end_offset);
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "end_offset"), "set_end_offset", "get_end_offset");
	ClassDB::bind_method(D_METHOD("set_anim_duration"), &FuncMeshMover::set_anim_duration);
	ClassDB::bind_method(D_METHOD("get_anim_duration"), &FuncMeshMover::get_anim_duration);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "anim_duration"), "set_anim_duration", "get_anim_duration");

	ClassDB::bind_method(D_METHOD("start"), &FuncMeshMover::start);
}

void FuncMeshMover::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	init_position = get_position();

	if (trigger) {
		trigger->connect("triggered", callable_mp(this, &FuncMeshMover::start));
	}
}

void FuncMeshMover::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (triggered) {
		t_anim += delta;
	}
	t_anim = Math::clamp(t_anim, 0.0, 1.0);

	Vector3 end_position = init_position + end_offset;
	set_position(init_position.lerp(end_position, t_anim / anim_duration));
}

void FuncMeshMover::start() {
	t_anim = 0.0;
	triggered = true;
}

} //namespace godot