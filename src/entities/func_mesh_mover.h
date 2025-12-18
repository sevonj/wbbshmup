#pragma once

#include <entities/func_trigger.h>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class FuncMeshMover : public Node3D {
	GDCLASS(FuncMeshMover, Node3D)

	// --- params
	FuncTrigger *trigger = nullptr;
	Vector3 end_offset = Vector3(0.0, -10.0, 0.0);
	double anim_duration = 2.0;

	// --- state
	Vector3 init_position;
	double t_anim = 0.0;
	bool triggered = false;

protected:
	static void _bind_methods();

public:
	FuncMeshMover() = default;
	~FuncMeshMover() override = default;

	void set_trigger(FuncTrigger *v) { trigger = v; }
	FuncTrigger *get_trigger() const { return trigger; }
	void set_end_offset(Vector3 v) { end_offset = v; }
	Vector3 get_end_offset() const { return end_offset; }
	void set_anim_duration(float v) { anim_duration = v; }
	float get_anim_duration() const { return anim_duration; }

	void _ready() override;
	void _process(double delta) override;

	void start();
};

} //namespace godot