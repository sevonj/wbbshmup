#include "anim_path_follow.h"

#include <godot_cpp/classes/engine.hpp>

namespace godot {

Ref<Curve3D> AnimPathFollow::get_curve() {
	Path3D *parent = cast_to<Path3D>(get_parent());
	if (parent) {
		return parent->get_curve();
	}
	return Ref<Curve3D>();
}

void AnimPathFollow::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_autostart"), &AnimPathFollow::get_autostart);
	ClassDB::bind_method(D_METHOD("set_autostart"), &AnimPathFollow::set_autostart);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autostart"), "set_autostart", "get_autostart");
	ClassDB::bind_method(D_METHOD("get_oneshot"), &AnimPathFollow::get_oneshot);
	ClassDB::bind_method(D_METHOD("set_oneshot"), &AnimPathFollow::set_oneshot);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "oneshot"), "set_oneshot", "get_oneshot");
	ClassDB::bind_method(D_METHOD("get_use_rotation"), &AnimPathFollow::get_use_rotation);
	ClassDB::bind_method(D_METHOD("set_use_rotation"), &AnimPathFollow::set_use_rotation);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_rotation"), "set_use_rotation", "get_use_rotation");
	ClassDB::bind_method(D_METHOD("get_run_in_editor"), &AnimPathFollow::get_run_in_editor);
	ClassDB::bind_method(D_METHOD("set_run_in_editor"), &AnimPathFollow::set_run_in_editor);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "run_in_editor"), "set_run_in_editor", "get_run_in_editor");
	ClassDB::bind_method(D_METHOD("get_running"), &AnimPathFollow::get_running);
	ClassDB::bind_method(D_METHOD("set_running"), &AnimPathFollow::set_running);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "running"), "set_running", "get_running");
	ClassDB::bind_method(D_METHOD("get_speed"), &AnimPathFollow::get_speed);
	ClassDB::bind_method(D_METHOD("set_speed"), &AnimPathFollow::set_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
	ClassDB::bind_method(D_METHOD("get_progress"), &AnimPathFollow::get_progress);
	ClassDB::bind_method(D_METHOD("set_progress"), &AnimPathFollow::set_progress);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "progress"), "set_progress", "get_progress");
	ClassDB::bind_method(D_METHOD("get_progress_ratio"), &AnimPathFollow::get_progress_ratio);
	ClassDB::bind_method(D_METHOD("set_progress_ratio"), &AnimPathFollow::set_progress_ratio);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "progress_ratio", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_progress_ratio", "get_progress_ratio");
	ClassDB::bind_method(D_METHOD("play"), &AnimPathFollow::play);
	ClassDB::bind_method(D_METHOD("stop"), &AnimPathFollow::stop);
	ClassDB::bind_method(D_METHOD("pause"), &AnimPathFollow::pause);
	ClassDB::bind_method(D_METHOD("restart"), &AnimPathFollow::restart);
}

AnimPathFollow::AnimPathFollow() {
}

AnimPathFollow::~AnimPathFollow() {}

void AnimPathFollow::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	running = false;
	progress = 0.0;

	if (autostart) {
		call_deferred("play");
	}
}

void AnimPathFollow::_process(double delta) {
	Ref<Curve3D> curve = get_curve();
	if (curve == nullptr) {
		set_position(Vector3());
		if (running) {
			print_error(get_class_static(), ": Failed to get curve!");
			running = false;
		}
		return;
	} else if (curve->get_point_count() == 0) {
		set_position(Vector3());
		if (running) {
			print_error(get_class_static(), ": Curve has no points!");
			running = false;
		}
		return;
	} else if (curve->get_point_count() == 1) {
		set_position(curve->get_point_position(0));
		if (running) {
			print_error(get_class_static(), ": Curve only has 1 point!");
			running = false;
		}
		return;
	}

	float length = curve->get_baked_length();
	if (progress > length) {
		if (oneshot) {
			running = false;
			return;
		}
		progress -= length;
	}

	if (use_rotation) {
		set_transform(curve->sample_baked_with_rotation(progress, false, true));
	} else {
		set_position(curve->sample_baked(progress));
	}

	if (Engine::get_singleton()->is_editor_hint() && !run_in_editor) {
		return;
	}

	if (running) {
		progress += delta * speed;
	}
}

/// @brief Same as get_progress, but adjusted to path length.
/// @return between 0.0 and 1.0. zero if no curve.
double AnimPathFollow::get_progress_ratio() {
	Ref<Curve3D> curve = get_curve();
	if (curve == nullptr) {
		print_error(get_class_static(), ": Failed to get curve!");
		return 0.;
	}
	return progress / curve->get_baked_length();
}

/// @brief Same as get_progress, but adjusted to path length.
/// @param value 0.0 and 1.0 map to path start and end.
void AnimPathFollow::set_progress_ratio(double value) {
	Ref<Curve3D> curve = get_curve();
	if (curve == nullptr) {
		print_error(get_class_static(), ": Failed to get curve!");
		return;
	}
	progress = value * curve->get_baked_length();
}

void AnimPathFollow::play() {
	running = true;
}

void AnimPathFollow::stop() {
	progress = 0.;
	running = false;
}

void AnimPathFollow::pause() {
	running = false;
}

void AnimPathFollow::restart() {
	progress = 0.;
}

} //namespace godot