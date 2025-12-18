#pragma once

#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/path3d.hpp>
#include <godot_cpp/classes/surface_tool.hpp>

namespace mesh_gen {

using namespace godot;

class PdmMesh : public RefCounted {
public:
	Ref<ArrayMesh> mesh;
	AABB aabb;
	String name;
};

class PdmAsset : public RefCounted {
public:
	Ref<PdmMesh> cap_start;
	Ref<PdmMesh> cap_end;
	TypedArray<PdmMesh> pieces;
};

} //namespace mesh_gen

namespace godot {

using namespace mesh_gen;

class ToolPathDeformMesh : public MeshInstance3D {
	GDCLASS(ToolPathDeformMesh, MeshInstance3D)

	static constexpr float INVALID_MESH_SKIP_LEN = 1.0;

	/// @brief This would be your blend file.
	Ref<PackedScene> pdm_asset_packed;
	Ref<PdmAsset> pdm_asset;

	/// @brief Rebuild on curve_changed signal
	bool enable_auto_rebuild = false;
	bool enable_cap_start = true;
	bool enable_cap_end = true;
	/// @brief Determines fill pattern. A-Z, case insensitive. "ACDC" repeats pieces 0,2,3,2,...
	String cadence_str = "A";
	double start_offset = 0.0;
	double end_offset = 0.0;

protected:
	static void _bind_methods();

public:
	ToolPathDeformMesh();
	~ToolPathDeformMesh() override = default;

	void set_pdm_asset(Ref<PackedScene> v) {
		pdm_asset_packed = v;
		pdm_asset = unpack_asset(v);
		auto_rebuild();
	}
	Ref<PackedScene> get_pdm_asset() const { return pdm_asset_packed; }

	void set_enable_auto_rebuild(bool v) { set_and_auto_rebuild(enable_auto_rebuild, v); }
	bool get_enable_auto_rebuild() const { return enable_auto_rebuild; }
	void set_enable_cap_start(bool v) { set_and_auto_rebuild(enable_cap_start, v); }
	bool get_enable_cap_start() const { return enable_cap_start; }
	void set_enable_cap_end(bool v) { set_and_auto_rebuild(enable_cap_end, v); }
	bool get_enable_cap_end() const { return enable_cap_end; }
	void set_cadence_str(String v) { set_and_auto_rebuild(cadence_str, v); }
	String get_cadence_str() const { return cadence_str; }
	void set_start_offset(double v) { set_and_auto_rebuild(start_offset, v); }
	double get_start_offset() const { return start_offset; }
	void set_end_offset(double v) { set_and_auto_rebuild(end_offset, v); }
	double get_end_offset() const { return end_offset; }
	void set_start_offset_ratio(double v) {
		Ref<Curve3D> curve = get_curve();
		if (!curve.is_valid()) {
			return;
		}
		start_offset = v * curve->get_baked_length();
		auto_rebuild();
	}
	double get_start_offset_ratio() {
		Ref<Curve3D> curve = get_curve();
		if (!curve.is_valid()) {
			return 0.;
		}
		return start_offset / curve->get_baked_length();
	}
	void set_end_offset_ratio(double v) {
		Ref<Curve3D> curve = get_curve();
		if (!curve.is_valid()) {
			return;
		}
		end_offset = v * curve->get_baked_length();
		auto_rebuild();
	}
	double get_end_offset_ratio() {
		Ref<Curve3D> curve = get_curve();
		if (!curve.is_valid()) {
			return 0.;
		}
		return end_offset / curve->get_baked_length();
	}
	bool has_start_cap() { return pdm_asset.is_valid() && pdm_asset->cap_start.is_valid(); }
	bool has_end_cap() { return pdm_asset.is_valid() && pdm_asset->cap_end.is_valid(); }
	int32_t get_num_pieces() {
		if (pdm_asset.is_valid()) {
			return pdm_asset->pieces.size();
		}
		return 0;
	}

	virtual void _enter_tree() override;
	virtual void _exit_tree() override;

	void rebuild_mesh();

private:
	Ref<Curve3D> get_curve();
	Vector<int32_t> get_cadence();
	Ref<PdmAsset> unpack_asset(Ref<PackedScene> packed);
	AABB calculate_arraymesh_aabb(Ref<ArrayMesh> mesh);

	/// @brief Bake a deformed mesh to surface tool
	/// @param st
	/// @param src_mesh
	/// @param z_scale Multiplier for vertex Z coord. Used for stretchin the mesh a lil bit to fit the path length exactly.
	/// @param deform_path
	/// @param path_offset Mesh's origin point will be at this position in the curve
	/// @param indices_start Number of vertices already present in this mesh
	/// @return Number of vertices added to the mesh
	int32_t bake_mesh(
			Ref<SurfaceTool> st,
			Ref<PdmMesh> src_mesh,
			float stretch,
			Ref<Curve3D> deform_path,
			double path_offset,
			int32_t indices_start);

	/// @brief Used by setters and curve_changed signal callback
	void auto_rebuild() {
		if (enable_auto_rebuild) {
			rebuild_mesh();
		}
	}

	template <typename T>
	void set_and_auto_rebuild(T &field, T v) {
		field = v;
		auto_rebuild();
	}
};

} //namespace godot