#include "camera_rig_follow.h"

#include <config.h>
#include <entities/player.h>
#include <game.h>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

void CameraRigFollow::_bind_methods() {}

CameraRigFollow::CameraRigFollow() {
	target = nullptr;
}

CameraRigFollow::~CameraRigFollow() {
}

Node3D *CameraRigFollow::get_target() {
	return target;
}

void CameraRigFollow::set_target(Node3D *node) {
	target = node;
}

void CameraRigFollow::_ready() {
	CameraRig::_ready();

	set_name("camera_rig_follow");
}

void CameraRigFollow::_process(double delta) {
	_process_camera_arm(delta);
}

void CameraRigFollow::_process_camera_arm(double delta) {
	if (!target) {
		return;
	}

	set_global_transform(target->get_global_transform());
	translate(CAMERA_OFFSET);
	camera->set_rotation_degrees(Vector3(-22.4, 0., 0.));
}

} //namespace godot