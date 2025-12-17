#pragma once

#include <consts.h>
#include <entities/camera_rig.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class CameraRigTp : public CameraRig {
	GDCLASS(CameraRigTp, CameraRig)

	static constexpr float FOLLOW_SPEED = 4.0;
	static constexpr float PITCH_MAX_ANGLE = 45.0 * DEG_TO_RAD;
	static constexpr float PITCH_MIN_ANGLE = -60.0 * DEG_TO_RAD;
	static constexpr float PITCH_DEFAULT_ANGLE = -10.0 * DEG_TO_RAD;
	static constexpr float PITCH_RETURN_SPEED = 1.0;
	static constexpr float CAMERA_DIST = 4.0;
	static inline const Vector3 CAMERA_OFFSET = Vector3(0.0, 0.5, 0.0);

	Node3D *pivot = nullptr;

protected:
	static void _bind_methods();
	void setup_rig() override;

public:
	CameraRigTp() = default;
	~CameraRigTp() override = default;

	void _ready() override;
	void _process(double delta) override;
	void _input(const Ref<InputEvent> &event) override;

private:
	void _process_controller_input(double delta);
	void _process_camera_arm(double delta);

	void clamp_rotation();
};

} //namespace godot
