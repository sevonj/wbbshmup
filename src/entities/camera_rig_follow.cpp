#include "camera_rig_follow.h"

#include <config.h>
#include <entities/player.h>
#include <game.h>
#include <stage.h>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

void CameraRigFollow::_bind_methods() {}

void CameraRigFollow::_ready() {
	CameraRig::_ready();

	set_name("camera_rig_follow");
}

void CameraRigFollow::_process(double delta) {
	_process_camera_arm(delta);
}

void CameraRigFollow::_process_camera_arm(double delta) {
	Player *player = Game::get_player();
	Stage *stage = Game::get_stage();
	if (!player || !stage) {
		return;
	}

	const Transform3D rail_xform = stage->sample_rail_at_time(player->get_t_stage());
	const Vector3 rail_offset_x = player->get_rail_offset() * Vector3(1., 0., 0.);

	set_global_transform(rail_xform);
	translate(CAMERA_OFFSET);
	translate(rail_offset_x * RAIL_OFFSET_SCALE);

	camera->set_rotation_degrees(Vector3(-22.4, 0., 0.));
}

} //namespace godot