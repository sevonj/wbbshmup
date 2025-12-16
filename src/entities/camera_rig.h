#pragma once

#include <consts.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class CameraRig : public Node3D {
	GDCLASS(CameraRig, Node3D)

protected:
	static void _bind_methods();

public:
	CameraRig() = default;
	~CameraRig() override;

	bool get_enabled() const { return enabled; }
	void set_enabled(bool v) { enabled = v; }
	Camera3D *get_camera() const { return camera; }
	Basis get_look_basis() const { return get_camera()->get_global_basis(); }

	void _ready() override;

protected:
	bool enabled = true;

	Camera3D *camera = nullptr;

	virtual void setup_rig();
};

} //namespace godot
