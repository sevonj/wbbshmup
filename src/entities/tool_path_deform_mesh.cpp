#include "tool_path_deform_mesh.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/material.hpp>

namespace godot {

void ToolPathDeformMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_pdm_set"), &ToolPathDeformMesh::set_pdm_set);
	ClassDB::bind_method(D_METHOD("get_pdm_set"), &ToolPathDeformMesh::get_pdm_set);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pdm_set"), "set_pdm_set", "get_pdm_set");

	ClassDB::bind_method(D_METHOD("rebuild_mesh"), &ToolPathDeformMesh::rebuild_mesh);
}

void ToolPathDeformMesh::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
}

Ref<Curve3D> ToolPathDeformMesh::get_curve() {
	Path3D *parent = cast_to<Path3D>(get_parent());
	if (parent) {
		return parent->get_curve();
	}
	return Ref<Curve3D>();
}

TypedArray<ArrayMesh> ToolPathDeformMesh::get_meshes() {
	TypedArray<ArrayMesh> meshes = TypedArray<ArrayMesh>();
	if (!pdm_set.is_valid()) {
		print_error(get_class_static(), ": Invalid PDM set!");
		return meshes;
	}

	Node *set_instance = pdm_set->instantiate();

	TypedArray<MeshInstance3D> pdm_meshinsts = TypedArray<MeshInstance3D>();
	TypedArray<Node> nodes = TypedArray<Node>();
	nodes.append(set_instance);

	while (!nodes.is_empty()) {
		Node *node = cast_to<Node>(nodes.pop_front());
		nodes.append_array(node->get_children());
		MeshInstance3D *meshinst = cast_to<MeshInstance3D>(node);
		if (meshinst) {
			pdm_meshinsts.append(meshinst);
		}
	}

	if (pdm_meshinsts.is_empty()) {
		print_error(get_class_static(), ": Found no models in PDM set!");
		return meshes;
	}
	int64_t set_size = pdm_meshinsts.size();
	print_line(get_class_static(), ": Found '", set_size, "' models in PDM set.");

	for (int i = 0; i < set_size; i++) {
		MeshInstance3D *inst = cast_to<MeshInstance3D>(pdm_meshinsts.pop_front());
		StringName mesh_name = inst->get_name();
		Ref<ArrayMesh> mesh = inst->get_mesh();
		if (!mesh.is_valid()) {
			continue;
		}
		meshes.append(mesh);
	}

	memdelete(set_instance);
	return meshes;
}

/// @return Distance from minimum z to maximum z.
float ToolPathDeformMesh::get_mesh_length(Ref<ArrayMesh> mesh) {
	int32_t num_surfaces = mesh->get_surface_count();
	print_line("num_surfaces: ", num_surfaces);

	float min = INFINITY;
	float max = -INFINITY;

	for (int surf = 0; surf < num_surfaces; surf++) {
		Array arrays = mesh->surface_get_arrays(surf);
		Array vertices = arrays[Mesh::ARRAY_VERTEX];

		if (vertices.is_empty()) {
			return 0.0;
		}

		int32_t num_v = vertices.size();

		for (int v = 0; v < num_v; v++) {
			Vector3 vertex = vertices[v];
			min = Math::min(min, vertex.z);
			max = Math::max(max, vertex.z);
		}
	}
	return max - min;
}

/// @brief Bake a deformed mesh to surface tool
/// @param st
/// @param src_mesh
/// @param src_mesh_len Source mesh size in Z axis
/// @param z_scale Multiplier for vertex Z coord. Used for stretchin the mesh a lil bit to fit the path length exactly.
/// @param deform_path
/// @param path_offset Mesh's origin point will be at this position in the curve
/// @param indices_start Number of vertices already present in this mesh
/// @return Number of vertices added to the mesh
int32_t ToolPathDeformMesh::add_mesh(
		Ref<SurfaceTool> st,
		Ref<ArrayMesh> src_mesh,
		float src_mesh_len,
		float z_scale,
		Ref<Curve3D> deform_path,
		double path_offset,
		int32_t indices_start) {
	int32_t num_surfaces = src_mesh->get_surface_count();
	print_line("num_surfaces: ", num_surfaces);

	int32_t num_v_added = 0;

	for (int surf = 0; surf < num_surfaces; surf++) {
		Mesh::PrimitiveType p_type = src_mesh->surface_get_primitive_type(surf);
		if (p_type != Mesh::PRIMITIVE_TRIANGLES) {
			print_error("Surf '", surf, "' has unhandled primitive type '", p_type, "'. Expected PRIMITIVE_TRIANGLES");
			continue;
		}

		Ref<Material> mat = src_mesh->surface_get_material(surf);
		st->set_material(mat);

		Array arrays = src_mesh->surface_get_arrays(surf);
		Array vertices = arrays[Mesh::ARRAY_VERTEX];
		Array normals = arrays[Mesh::ARRAY_NORMAL];
		Array tangents = arrays[Mesh::ARRAY_TANGENT];
		Array colors = arrays[Mesh::ARRAY_COLOR];
		Array uvs = arrays[Mesh::ARRAY_TEX_UV];
		Array uv2s = arrays[Mesh::ARRAY_TEX_UV2];
		// Array custom0s = arrays[Mesh::ARRAY_CUSTOM0];
		// Array custom1s = arrays[Mesh::ARRAY_CUSTOM1];
		// Array custom2s = arrays[Mesh::ARRAY_CUSTOM2];
		// Array custom3s = arrays[Mesh::ARRAY_CUSTOM3];
		// Array bones = arrays[Mesh::ARRAY_BONES];
		// Array weights = arrays[Mesh::ARRAY_WEIGHTS];
		Array indices = arrays[Mesh::ARRAY_INDEX];

		bool use_normals = normals.size() == vertices.size();
		bool use_tangents = tangents.size() == vertices.size();
		bool use_colors = colors.size() == vertices.size();
		bool use_uvs = uvs.size() == vertices.size();
		bool use_uv2s = uv2s.size() == vertices.size();

		int32_t num_v = vertices.size();
		int32_t num_i = indices.size();

		for (int v = 0; v < num_v; v++) {
			if (use_normals) {
				st->set_normal(normals[v]);
			}
			if (use_tangents) {
				st->set_tangent(tangents[v]);
			}
			if (use_colors) {
				st->set_color(colors[v]);
			}
			if (use_uvs) {
				st->set_uv(uvs[v]);
			}
			if (use_uv2s) {
				st->set_uv2(uv2s[v]);
			}
			Vector3 vertex = vertices[v];
			float vert_z = (src_mesh_len - vertex.z) * z_scale;
			Transform3D xform = deform_path->sample_baked_with_rotation(path_offset + vert_z);
			vertex.z = 0.0;
			st->add_vertex(xform.xform(vertex));
		}

		for (int i = 0; i < num_i; i++) {
			int32_t index = indices[i];
			st->add_index(index + indices_start);
		}

		num_v_added += num_v;
	}

	return num_v_added;
}

void ToolPathDeformMesh::rebuild_mesh() {
	print_error(get_class_static(), ": Building..");

	Ref<Curve3D> curve = get_curve();
	if (curve == nullptr) {
		set_position(Vector3());
		print_error(get_class_static(), ": Failed to get curve!");
		return;
	} else if (curve->get_point_count() == 0) {
		set_position(Vector3());
		print_error(get_class_static(), ": Curve has no points!");
		return;
	} else if (curve->get_point_count() == 1) {
		set_position(curve->get_point_position(0));
		print_error(get_class_static(), ": Curve only has 1 point!");
		return;
	}

	TypedArray<ArrayMesh> meshes = get_meshes();
	if (meshes.is_empty()) {
		return;
	}

	Ref<SurfaceTool> st = memnew(SurfaceTool);
	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	Ref<ArrayMesh> src_mesh = meshes[0];
	Ref<ArrayMesh> gen_mesh = memnew(ArrayMesh);
	float path_len = curve->get_baked_length();
	float segment_len = get_mesh_length(src_mesh);
	int32_t num_segments = path_len / segment_len;
	float z_scale = (path_len / num_segments) / segment_len;

	int32_t num_v = 0;
	for (int i = 0; i < num_segments; i++) {
		double path_offset = segment_len * i * z_scale;
		num_v += add_mesh(st, src_mesh, segment_len, z_scale, curve, path_offset, num_v);
	}
	st->optimize_indices_for_cache();
	set_mesh(st->commit());
}

} //namespace godot