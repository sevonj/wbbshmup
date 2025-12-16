#pragma once

#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/path3d.hpp>
#include <godot_cpp/classes/surface_tool.hpp>

namespace mesh_gen {

using namespace godot;

struct PdmMesh : public RefCounted {
	Ref<ArrayMesh> mesh = Ref<ArrayMesh>();
	AABB aabb = AABB();
	String name = "";
};

struct PdmAsset : public RefCounted {
	Ref<PdmMesh> cap_start = Ref<PdmMesh>();
	Ref<PdmMesh> cap_end = Ref<PdmMesh>();
	TypedArray<PdmMesh> pieces = TypedArray<PdmMesh>();
};

} //namespace mesh_gen

namespace godot {

using namespace mesh_gen;

class ToolPathDeformMesh : public MeshInstance3D {
	GDCLASS(ToolPathDeformMesh, MeshInstance3D)

private:
	/// @brief This would be your blend file.
	Ref<PackedScene> pdm_asset_packed = Ref<PackedScene>();
	Ref<PdmAsset> pdm_asset = Ref<PdmAsset>();
	/// @brief Rebuild on curve_changed signal
	bool enable_auto_rebuild = false;
	bool enable_cap_start = true;
	bool enable_cap_end = true;
	/// @brief Determines fill pattern. A-Z, case insensitive. "ACDC" repeats pieces 0,2,3,2,...
	String cadence_str = "A";
	double start_offset = 0.0;
	double end_offset = 0.0;

	Ref<Curve3D> get_curve();
	Vector<int32_t> get_cadence();
	Ref<PdmAsset> unpack_asset();
	AABB calculate_arraymesh_aabb(Ref<ArrayMesh> mesh);
	int32_t add_mesh(
			Ref<SurfaceTool> st,
			Ref<PdmMesh> src_mesh,
			float stretch,
			Ref<Curve3D> deform_path,
			double path_offset,
			int32_t indices_start);

	void auto_rebuild();

protected:
	static void _bind_methods();

public:
	ToolPathDeformMesh();
	~ToolPathDeformMesh() {};

	virtual void _enter_tree() override;
	virtual void _exit_tree() override;

	void set_pdm_asset(Ref<PackedScene> v);
	Ref<PackedScene> get_pdm_asset() { return pdm_asset_packed; };
	void set_enable_auto_rebuild(bool v) {
		auto_rebuild();
		enable_auto_rebuild = v;
	};
	bool get_enable_auto_rebuild() { return enable_auto_rebuild; };
	void set_enable_cap_start(bool v) {
		auto_rebuild();
		enable_cap_start = v;
	};
	bool get_enable_cap_start() { return enable_cap_start; };
	void set_enable_cap_end(bool v) {
		auto_rebuild();
		enable_cap_end = v;
	};
	bool get_enable_cap_end() { return enable_cap_end; };
	void set_cadence_str(String v) {
		auto_rebuild();
		cadence_str = v;
	};
	String get_cadence_str() { return cadence_str; };
	void set_start_offset(double v) {
		auto_rebuild();
		start_offset = v;
	};
	double get_start_offset() { return start_offset; };
	void set_end_offset(double v) {
		auto_rebuild();
		end_offset = v;
	};
	double get_end_offset() { return end_offset; };
	void set_start_offset_ratio(double v);
	double get_start_offset_ratio();
	void set_end_offset_ratio(double v);
	double get_end_offset_ratio();

	bool has_start_cap();
	bool has_end_cap();
	int32_t get_num_pieces();

	void rebuild_mesh();
};

} //namespace godot