#include "tool_path_deform_mesh.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/material.hpp>

using namespace mesh_gen;

namespace godot {

void ToolPathDeformMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_pdm_asset"), &ToolPathDeformMesh::set_pdm_asset);
	ClassDB::bind_method(D_METHOD("get_pdm_asset"), &ToolPathDeformMesh::get_pdm_asset);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "pdm_asset"), "set_pdm_asset", "get_pdm_asset");
	ClassDB::bind_method(D_METHOD("set_enable_auto_rebuild"), &ToolPathDeformMesh::set_enable_auto_rebuild);
	ClassDB::bind_method(D_METHOD("get_enable_auto_rebuild"), &ToolPathDeformMesh::get_enable_auto_rebuild);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_auto_rebuild"), "set_enable_auto_rebuild", "get_enable_auto_rebuild");
	ClassDB::bind_method(D_METHOD("set_enable_cap_start"), &ToolPathDeformMesh::set_enable_cap_start);
	ClassDB::bind_method(D_METHOD("get_enable_cap_start"), &ToolPathDeformMesh::get_enable_cap_start);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_cap_start"), "set_enable_cap_start", "get_enable_cap_start");
	ClassDB::bind_method(D_METHOD("set_enable_cap_end"), &ToolPathDeformMesh::set_enable_cap_end);
	ClassDB::bind_method(D_METHOD("get_enable_cap_end"), &ToolPathDeformMesh::get_enable_cap_end);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "enable_cap_end"), "set_enable_cap_end", "get_enable_cap_end");
	ClassDB::bind_method(D_METHOD("get_cadence_str"), &ToolPathDeformMesh::get_cadence_str);
	ClassDB::bind_method(D_METHOD("set_cadence_str"), &ToolPathDeformMesh::set_cadence_str);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "cadence"), "set_cadence_str", "get_cadence_str");
	ClassDB::bind_method(D_METHOD("get_start_offset"), &ToolPathDeformMesh::get_start_offset);
	ClassDB::bind_method(D_METHOD("set_start_offset"), &ToolPathDeformMesh::set_start_offset);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "start_offset"), "set_start_offset", "get_start_offset");
	ClassDB::bind_method(D_METHOD("get_end_offset"), &ToolPathDeformMesh::get_end_offset);
	ClassDB::bind_method(D_METHOD("set_end_offset"), &ToolPathDeformMesh::set_end_offset);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "end_offset"), "set_end_offset", "get_end_offset");
	ClassDB::bind_method(D_METHOD("get_start_offset_ratio"), &ToolPathDeformMesh::get_start_offset_ratio);
	ClassDB::bind_method(D_METHOD("set_start_offset_ratio"), &ToolPathDeformMesh::set_start_offset_ratio);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "start_offset_ratio", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_start_offset_ratio", "get_start_offset_ratio");
	ClassDB::bind_method(D_METHOD("get_end_offset_ratio"), &ToolPathDeformMesh::get_end_offset_ratio);
	ClassDB::bind_method(D_METHOD("set_end_offset_ratio"), &ToolPathDeformMesh::set_end_offset_ratio);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "end_offset_ratio", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_end_offset_ratio", "get_end_offset_ratio");

	ClassDB::bind_method(D_METHOD("has_start_cap"), &ToolPathDeformMesh::has_start_cap);
	ClassDB::bind_method(D_METHOD("has_end_cap"), &ToolPathDeformMesh::has_end_cap);
	ClassDB::bind_method(D_METHOD("get_num_pieces"), &ToolPathDeformMesh::get_num_pieces);

	ClassDB::bind_method(D_METHOD("rebuild_mesh"), &ToolPathDeformMesh::rebuild_mesh);
	ClassDB::bind_method(D_METHOD("auto_rebuild"), &ToolPathDeformMesh::auto_rebuild);
}

ToolPathDeformMesh::ToolPathDeformMesh() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (!pdm_asset.is_valid()) {
		pdm_asset = unpack_asset(pdm_asset_packed);
	}
}

void ToolPathDeformMesh::_enter_tree() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Path3D *parent = cast_to<Path3D>(get_parent());
	if (parent) {
		parent->connect("curve_changed", Callable(this, "auto_rebuild"));
	}

	/* Make mesh unique fix.
	 * When the node is copied and pasted in the editor, the newly created instance keeps original refs.
	 * We obviously don't want multiple PDMs fighting over the same mesh.
	 * And also I hate having to remember to click "make unqiue" in the inspector.
	 */
	Ref<ArrayMesh> mesh = get_mesh();
	if (mesh.is_valid()) {
		mesh = mesh->duplicate();
		set_mesh(mesh);
	}
}

void ToolPathDeformMesh::_exit_tree() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	Node *parent = get_parent();
	if (parent->is_connected("curve_changed", Callable(this, "auto_rebuild"))) {
		parent->disconnect("curve_changed", (this, Callable(this, "auto_rebuild")));
	}
}

void ToolPathDeformMesh::rebuild_mesh() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	Ref<Curve3D> curve = get_curve();
	if (!curve.is_valid()) {
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

	if (!pdm_asset.is_valid()) {
		print_error(get_class_static(), ": !pdm_asset.is_valid()");
		return;
	}

	float total_len = curve->get_baked_length() - start_offset - end_offset;

	Ref<PdmMesh> cap_start = pdm_asset->cap_start;
	Ref<PdmMesh> cap_end = pdm_asset->cap_end;
	bool use_cap_start = pdm_asset->cap_start.is_valid() && enable_cap_start;
	bool use_cap_end = pdm_asset->cap_end.is_valid() && enable_cap_end;
	double cap_len = 0.0;
	double cap_stretch_ratio = 1.0;
	if (use_cap_start) {
		cap_len += cap_start->aabb.size.z;
	}
	if (use_cap_end) {
		cap_len += cap_end->aabb.size.z;
	}
	if (cap_len > total_len) {
		cap_stretch_ratio = total_len / cap_len;
	}
	cap_len *= cap_stretch_ratio;

	double fill_length = total_len - cap_len;
	double fill_start = start_offset;
	double fill_end = fill_start + fill_length;
	if (use_cap_start) {
		fill_start += cap_start->aabb.size.z * cap_stretch_ratio;
	}
	if (use_cap_end) {
		fill_end -= cap_end->aabb.size.z * cap_stretch_ratio;
	}

	Vector<int32_t> fill_pattern;
	Vector<double> fill_offsets;

	Vector<int32_t> cadence = get_cadence();
	int32_t cadence_iter = 0;
	double fill_offset_unscaled = 0.0;
	while (fill_offset_unscaled < fill_end - fill_start) {
		int32_t mesh_idx = cadence[cadence_iter];
		if (mesh_idx >= pdm_asset->pieces.size()) {
			fill_offset_unscaled += INVALID_MESH_SKIP_LEN;
			continue;
		}
		Ref<PdmMesh> piece = cast_to<PdmMesh>(pdm_asset->pieces[mesh_idx]);
		double piece_len = piece->aabb.size.z;
		if (Math::is_zero_approx(piece_len)) {
			fill_offset_unscaled += INVALID_MESH_SKIP_LEN;
			continue;
		}
		fill_pattern.push_back(mesh_idx);
		fill_offsets.push_back(fill_offset_unscaled);
		fill_offset_unscaled += piece_len;
		cadence_iter++;
		cadence_iter %= cadence.size();
	}
	double fill_stretch = fill_length / fill_offset_unscaled;

	Ref<SurfaceTool> st = memnew(SurfaceTool);
	st->begin(Mesh::PRIMITIVE_TRIANGLES);
	Ref<ArrayMesh> gen_mesh = get_mesh();
	if (gen_mesh.is_valid()) {
		gen_mesh->clear_surfaces();
	} else {
		gen_mesh = (Ref<ArrayMesh>)memnew(ArrayMesh);
		set_mesh(gen_mesh);
	}

	int32_t num_v = 0;

	if (use_cap_start) {
		double path_offset = start_offset;
		num_v += bake_mesh(st, cap_start, cap_stretch_ratio, curve, path_offset, num_v);
	}

	if (use_cap_end) {
		double path_offset = start_offset + total_len - cap_end->aabb.size.z;
		num_v += bake_mesh(st, cap_end, cap_stretch_ratio, curve, path_offset, num_v);
	}

	for (int32_t i = 0; i < fill_pattern.size(); i++) {
		int32_t mesh_idx = fill_pattern[i];
		double path_offset = fill_start + fill_offsets[i] * fill_stretch;
		Ref<PdmMesh> src_mesh = pdm_asset->pieces[mesh_idx];
		num_v += bake_mesh(st, src_mesh, fill_stretch, curve, path_offset, num_v);
	}

	st->optimize_indices_for_cache();
	st->commit(gen_mesh);
}

Ref<Curve3D> ToolPathDeformMesh::get_curve() {
	Path3D *parent = cast_to<Path3D>(get_parent());
	if (parent) {
		return parent->get_curve();
	}
	return Ref<Curve3D>();
}

Vector<int32_t> ToolPathDeformMesh::get_cadence() {
	if (cadence_str.is_empty()) {
		print_error("Cadence was empty. Using default.");
		return { 0 };
	}
	Vector<int32_t> cadence = {};
	int len = cadence_str.length();
	for (int i = 0; i < len; i++) {
		char32_t c = cadence_str[i];
		if ('a' <= c && c < 'z') {
			cadence.push_back(c - 'a');
		} else if ('A' <= c && c < 'Z') {
			cadence.push_back(c - 'A');
		} else {
			print_error("Cadence has bad character: '", c, "' Use A-Z (case insensitive)");
			cadence.push_back(INT32_MAX);
		}
	}
	return cadence;
}

Ref<PdmAsset> ToolPathDeformMesh::unpack_asset(Ref<PackedScene> packed) {
	if (!packed.is_valid()) {
		print_error(get_class_static(), ": Invalid PDM set!");
		return Ref<PdmAsset>();
	}

	Node *set_instance = packed->instantiate();

	TypedArray<MeshInstance3D> pdm_meshinsts;
	TypedArray<Node> nodes;
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
		return Ref<PdmAsset>();
	}
	int64_t set_size = pdm_meshinsts.size();

	Ref<PdmAsset> asset = memnew(PdmAsset);

	for (int i = 0; i < set_size; i++) {
		MeshInstance3D *inst = cast_to<MeshInstance3D>(pdm_meshinsts.pop_front());
		StringName name = inst->get_name();
		Ref<ArrayMesh> mesh = inst->get_mesh();
		if (!mesh.is_valid()) {
			continue;
		}

		Ref<PdmMesh> pdm_mesh = memnew(PdmMesh);
		pdm_mesh->mesh = mesh;
		pdm_mesh->aabb = calculate_arraymesh_aabb(mesh);

		if (name == (StringName) "cap_start") {
			asset->cap_start = pdm_mesh;
		} else if (name == (StringName) "cap_end") {
			asset->cap_end = pdm_mesh;
		} else {
			asset->pieces.push_back(pdm_mesh);
		}
	}
	memdelete(set_instance);

	if (asset->pieces.is_empty()) {
		return Ref<PdmAsset>();
	}
	return asset;
}

AABB ToolPathDeformMesh::calculate_arraymesh_aabb(Ref<ArrayMesh> mesh) {
	int32_t num_surfaces = mesh->get_surface_count();

	Vector3 min = Vector3(INFINITY, INFINITY, INFINITY);
	Vector3 max = Vector3(-INFINITY, -INFINITY, -INFINITY);

	for (int surf = 0; surf < num_surfaces; surf++) {
		Array arrays = mesh->surface_get_arrays(surf);
		Array vertices = arrays[Mesh::ARRAY_VERTEX];
		if (vertices.is_empty()) {
			return AABB();
		}
		int32_t num_v = vertices.size();
		for (int v = 0; v < num_v; v++) {
			Vector3 vertex = vertices[v];
			min.x = Math::min(min.x, vertex.x);
			min.y = Math::min(min.y, vertex.y);
			min.z = Math::min(min.z, vertex.z);
			max.x = Math::max(max.x, vertex.x);
			max.y = Math::max(max.y, vertex.y);
			max.z = Math::max(max.z, vertex.z);
		}
	}
	return AABB(min, max - min);
}

int32_t ToolPathDeformMesh::bake_mesh(
		Ref<SurfaceTool> st,
		Ref<PdmMesh> src_mesh,
		float stretch,
		Ref<Curve3D> deform_path,
		double path_offset,
		int32_t indices_start) {
	int32_t num_surfaces = src_mesh->mesh->get_surface_count();

	int32_t num_v_added = 0;

	for (int surf = 0; surf < num_surfaces; surf++) {
		Mesh::PrimitiveType p_type = src_mesh->mesh->surface_get_primitive_type(surf);
		if (p_type != Mesh::PRIMITIVE_TRIANGLES) {
			print_error("Surf '", surf, "' has unhandled primitive type '", p_type, "'. Expected PRIMITIVE_TRIANGLES");
			continue;
		}

		Ref<Material> mat = src_mesh->mesh->surface_get_material(surf);
		st->set_material(mat);

		Array arrays = src_mesh->mesh->surface_get_arrays(surf);
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
			float vert_z = (src_mesh->aabb.size.z + src_mesh->aabb.position.z - vertex.z) * stretch;
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

} //namespace godot