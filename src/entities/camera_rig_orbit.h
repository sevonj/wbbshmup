#pragma once

#include <consts.h>
#include <entities/camera_rig.h>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class CameraRigOrbit : public CameraRig {
	GDCLASS(CameraRigOrbit, CameraRig)

	static constexpr double MIN_DIST = 3.0;
	static constexpr double MAX_DIST = 30.0;

	Node3D *pivot = nullptr;

	double distance = 0.0;

protected:
	static void _bind_methods();

public:
	CameraRigOrbit() = default;
	~CameraRigOrbit() override = default;

	void _ready() override;
	void _process(double delta) override;
	void _input(const Ref<InputEvent> &event) override;

protected:
	void setup_rig() override;

private:
	void _process_controller_input(double delta);
	void _process_camera_arm(double delta);
};

} //namespace godot
