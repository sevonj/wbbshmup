#pragma once

#include <consts.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

class CameraRig : public Node3D {
	GDCLASS(CameraRig, Node3D)

private:
protected:
	Camera3D *camera = nullptr;

	static void _bind_methods();
	virtual void setup_rig();

public:
	bool enabled = true;

	CameraRig();
	~CameraRig();

	void _ready() override;
	void _process(double delta) override;

	Camera3D *get_camera();
	Basis get_look_basis();

	bool get_enabled();
	void set_enabled(bool value);
};

} //namespace godot
