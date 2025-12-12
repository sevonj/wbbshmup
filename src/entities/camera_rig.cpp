#include "camera_rig.h"

#include <game.h>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

void CameraRig::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_enabled"), &CameraRig::get_enabled);
	ClassDB::bind_method(D_METHOD("set_enabled"), &CameraRig::set_enabled);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enabled", PROPERTY_HINT_NONE, "Camera input enabled"), "set_enabled", "get_enabled");

	ClassDB::bind_method(D_METHOD("get_camera"), &CameraRig::get_camera);
}

CameraRig::CameraRig() {
}

CameraRig::~CameraRig() {
	if (Game::get_current_camera() == this) {
		Game::set_current_camera(nullptr);
	}
}

void CameraRig::_ready() {
	set_name("camera_rig");

	camera = memnew(Camera3D);
	camera->set_name("camera3d");
	setup_rig();

	Game::get_singleton()->set_current_camera(this);
}

void CameraRig::_process(double delta) {
}

Camera3D *CameraRig::get_camera() {
	return camera;
}

Basis CameraRig::get_look_basis() {
	return get_camera()->get_global_basis();
}

bool CameraRig::get_enabled() {
	return enabled;
}

void CameraRig::set_enabled(bool value) {
	enabled = value;
}

void CameraRig::setup_rig() {
	add_child(camera);
}

} //namespace godot