#pragma once

#include <stage.h>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Simple animation tool. Makes a node move along a path. Parent must be a Path3D.
class ToolStagePathMarker : public Node3D {
	GDCLASS(ToolStagePathMarker, Node3D)

private:
	double time = 0.;

protected:
	static void _bind_methods();

public:
	ToolStagePathMarker() {};
	~ToolStagePathMarker() {};

	virtual void _process(double delta) override;

	double get_time() { return time; };
	void set_time(double v) { time = v; };
};

} //namespace godot