#pragma once

#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

namespace godot {

class StagePathGrid : public MeshInstance3D {
	GDCLASS(StagePathGrid, MeshInstance3D)

private:
	static constexpr double SEGMENT_LEN = 8.;
	static constexpr double GRID_WIDTH = 24.;
	const String MAT_PATH = "res://assets/materials/mat_grid.tres";

protected:
	static void _bind_methods();

public:
	StagePathGrid();
	~StagePathGrid();

	void rebuild_mesh(Ref<Curve3D> curve);
};

} //namespace godot