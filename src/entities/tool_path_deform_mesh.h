#pragma once

#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/path3d.hpp>
#include <godot_cpp/classes/surface_tool.hpp>

namespace godot {

class ToolPathDeformMesh : public MeshInstance3D {
	GDCLASS(ToolPathDeformMesh, MeshInstance3D)

private:
	Ref<PackedScene> pdm_asset = Ref<PackedScene>();

	Ref<Curve3D> get_curve();
	TypedArray<ArrayMesh> get_meshes();
	float get_mesh_length(Ref<ArrayMesh> mesh);
	int32_t add_mesh(
			Ref<SurfaceTool> st,
			Ref<ArrayMesh> src_mesh,
			float src_mesh_len,
			float z_scale,
			Ref<Curve3D> curve,
			double path_offset,
			int32_t indices_start);

protected:
	static void _bind_methods();

public:
	ToolPathDeformMesh() {};
	~ToolPathDeformMesh() {};

	virtual void _ready() override;

	void set_pdm_asset(Ref<PackedScene> v) { pdm_asset = v; };
	Ref<PackedScene> get_pdm_asset() { return pdm_asset; };

	void rebuild_mesh();
};

} //namespace godot