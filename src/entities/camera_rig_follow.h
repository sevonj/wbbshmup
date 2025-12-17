#pragma once

#include <consts.h>
#include <entities/camera_rig.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class CameraRigFollow : public CameraRig {
	GDCLASS(CameraRigFollow, CameraRig)

	static inline const Vector3 CAMERA_OFFSET = Vector3(0.0, 8.0, 13.25);

	Node3D *target = nullptr;

protected:
	static void _bind_methods();

public:
	CameraRigFollow() = default;
	~CameraRigFollow() override = default;

	void set_target(Node3D *v) { target = v; }
	Node3D *get_target() const { return target; }

	void _ready() override;
	void _process(double delta) override;

private:
	void _process_camera_arm(double delta);
};

} //namespace godot
