#pragma once

#include <consts.h>
#include <entities/camera_rig.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class CameraRigFollow : public CameraRig {
	GDCLASS(CameraRigFollow, CameraRig)

private:
	const Vector3 CAMERA_OFFSET = Vector3(0., 8., 13.25);

	Node3D *target;

	void _process_camera_arm(double delta);

protected:
	static void _bind_methods();

public:
	CameraRigFollow();
	~CameraRigFollow();

	Node3D *get_target();
	void set_target(Node3D *node);

	void _ready() override;
	void _process(double delta) override;
};

} //namespace godot
