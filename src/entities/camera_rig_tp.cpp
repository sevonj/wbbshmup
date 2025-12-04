#include "camera_rig_tp.h"

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

void CameraRigTp::_bind_methods() {}

CameraRigTp::CameraRigTp() {
	pivot = nullptr;
}

CameraRigTp::~CameraRigTp() {
}

void CameraRigTp::_ready() {
	pivot = memnew(Node3D);

	set_name("camera_rig_tp");

	setup_rig();
}

void CameraRigTp::_process(double delta) {
	_process_controller_input(delta);
	_process_camera_arm(delta);
}

void CameraRigTp::_process_controller_input(double delta) {
	Input *input = Input::get_singleton();

	// TODO: move to settings
	Vector2 input_controller_sens = Vector2(2., 1.5);

	float x = input->get_axis("look_right", "look_left");
	float y = input->get_axis("look_down", "look_up");

	if (config::input_invert_x) {
		x *= -1;
	}
	if (config::input_invert_y) {
		x *= -1;
	}

	rotate_y(x * delta * input_controller_sens.x);
	pivot->rotate_x(y * delta * input_controller_sens.y);

	clamp_rotation();
}

void CameraRigTp::_process_camera_arm(double delta) {
	Player *player = Game::get_player();

	if (!player) {
		return;
	}

	Input *input = Input::get_singleton();
	bool strafing = input->is_action_pressed("strafe");
	Vector3 player_position = player->get_focus_point() + Vector3(0, 0.4, 0);
	Vector3 camera_position = camera->get_global_position();
	Vector3 rig_position = get_global_position();
	Basis rig_basis = get_global_basis();

	// Follow Player
	rig_position -= (rig_position - player_position) * FOLLOW_SPEED * delta;

	// Pan slightly towards player
	if (!strafing) {
		Vector3 pos_diff = player_position - camera_position;
		float target_angle = -Vector2(-pos_diff.z, pos_diff.x).angle();
		float angle_diff = target_angle - get_rotation().y;
		if (fabs(angle_diff) > M_PI) {
			angle_diff += 2 * M_PI * -Math::sign(angle_diff);
		}
		double delta_angle = 1.5 * delta * Math::sign(angle_diff); // * player->get_velocity().length();
		delta_angle = Math::clamp(delta_angle, -fabs(angle_diff), fabs(angle_diff));
		rotate_y(delta_angle);
	}

	// Truck sideways so that target is exactly centered on screen (horizontally). Centering the
	// target just by panning would be way too intense.
	rig_position += rig_basis.get_column(0) * (get_transform().xform_inv(player_position)).x;

	// Raycast backwards to get camera distace
	Vector3 end = rig_position + get_look_basis().get_column(2) * CAMERA_DIST;
	Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(rig_position, end, COL_MASK_CAMERA);
	PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
	Dictionary result = space_state->intersect_ray(query);
	float distance = CAMERA_DIST;
	if (!result.is_empty()) {
		Vector3 result_position = result.get("position", Vector3(0, 0, 0));
		distance = rig_position.distance_to(result_position);
	}

	// Slow return to default tilt
	if (!strafing) {
		Vector3 pivot_rotation = pivot->get_rotation();
		pivot_rotation.x -= (pivot_rotation.x - PITCH_DEFAULT_ANGLE) * PITCH_RETURN_SPEED * delta;
		pivot->set_rotation(pivot_rotation);
	}

	camera->set_position(Vector3(0, CAMERA_OFFSET.y, distance));
	set_global_position(rig_position);
}

void CameraRigTp::_input(const Ref<InputEvent> &event) {
	if (!enabled) {
		return;
	}
	// TODO: paused guard

	Ref<InputEventMouseMotion> mouse_motion = event;
	if (mouse_motion.is_valid()) {
		float input_mouse_sens = 1.0; // TODO
		Vector2 relative = mouse_motion->get_relative();
		rotate_y(-relative.x * .01 * input_mouse_sens);
		pivot->rotate_x(-relative.y * .01 * input_mouse_sens);
		clamp_rotation();
		return;
	}
}

void CameraRigTp::clamp_rotation() {
	Vector3 rotation = pivot->get_rotation();
	rotation.x = Math::clamp(rotation.x, PITCH_MIN_ANGLE, PITCH_MAX_ANGLE);
	pivot->set_rotation(rotation);
}

void CameraRigTp::setup_rig() {
	pivot->set_name("pivot");
	remove_child(camera);
	add_child(pivot);
	pivot->add_child(camera);
}

} //namespace godot