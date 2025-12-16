#pragma once

#include <stage.h>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Simple animation tool. Makes a node move along a path. Parent must be a Path3D.
class ToolStagePathMarker : public Node3D {
	GDCLASS(ToolStagePathMarker, Node3D)

	double time = 0.;

protected:
	static void _bind_methods();

public:
	ToolStagePathMarker() = default;
	~ToolStagePathMarker() override = default;

	double get_time() { return time; };
	void set_time(double v) { time = v; };

	virtual void _process(double delta) override;
};

} //namespace godot