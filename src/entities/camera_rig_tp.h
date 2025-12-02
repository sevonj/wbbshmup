#pragma once

#include <consts.h>
#include <entities/camera_rig.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class CameraRigTp : public CameraRig {
	GDCLASS(CameraRigTp, CameraRig)

private:
	const float FOLLOW_SPEED = 4.0;
	const float PITCH_MAX_ANGLE = Math::deg_to_rad(45.0);
	const float PITCH_MIN_ANGLE = Math::deg_to_rad(-60.0);
	const float PITCH_DEFAULT_ANGLE = Math::deg_to_rad(-10.0);
	const float PITCH_RETURN_SPEED = 1.0;
	const float CAMERA_DIST = 4.0;
	const Vector3 CAMERA_OFFSET = Vector3(0.0, 0.5, 0.0);

	Node3D *pivot;

	void _process_controller_input(double delta);
	void _process_camera_arm(double delta);

	void clamp_rotation();

protected:
	static void _bind_methods();
	void setup_rig() override;

public:
	CameraRigTp();
	~CameraRigTp();

	void _ready() override;
	void _process(double delta) override;
	void _input(const Ref<InputEvent> &event) override;
};

} //namespace godot
