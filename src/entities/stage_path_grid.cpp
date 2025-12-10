#include "stage_path_grid.h"

#include <assets.h>
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/surface_tool.hpp>

namespace godot {

void StagePathGrid::_bind_methods() {}

StagePathGrid::StagePathGrid() {
}

StagePathGrid::~StagePathGrid() {}

void StagePathGrid::rebuild_mesh(Ref<Curve3D> curve) {
	const Vector3 RIGHT = Vector3(1., 0., 0.);

	double total_len = curve->get_baked_length();
	int32_t num_segs = total_len / SEGMENT_LEN;
	if (num_segs < 1) {
		num_segs = 1;
	}

	Ref<SurfaceTool> st = memnew(SurfaceTool);
	st->begin(Mesh::PRIMITIVE_TRIANGLES);

	Ref<Material> mat = ResourceLoader::get_singleton()->load(MAT_PATH, "Material");
	if (mat != nullptr) {
		st->set_material(mat);
	}

	for (int seg_idx = 0; seg_idx < num_segs - 1; seg_idx++) {
		double curr_offset = (double)seg_idx / num_segs * total_len;
		double next_offset = ((double)seg_idx + 1.) / num_segs * total_len;
		Transform3D curr = curve->sample_baked_with_rotation(curr_offset);
		Transform3D next = curve->sample_baked_with_rotation(next_offset);

		Vector3 v0 = curr.xform(-RIGHT * GRID_WIDTH / 2);
		Vector3 v1 = next.xform(-RIGHT * GRID_WIDTH / 2);
		Vector3 v2 = curr.get_origin();
		Vector3 v3 = next.get_origin();
		Vector3 v4 = curr.xform(RIGHT * GRID_WIDTH / 2);
		Vector3 v5 = next.xform(RIGHT * GRID_WIDTH / 2);

		Vector2 uv0 = Vector2(-1., 0.);
		Vector2 uv1 = Vector2(-1., -1.);
		Vector2 uv2 = Vector2(0., 0.);
		Vector2 uv3 = Vector2(0., -1.);
		Vector2 uv4 = Vector2(1., 0.);
		Vector2 uv5 = Vector2(1., -1.);

		st->set_uv(uv0);
		st->add_vertex(v0);
		st->set_uv(uv1);
		st->add_vertex(v1);
		st->set_uv(uv2);
		st->add_vertex(v2);
		st->set_uv(uv3);
		st->add_vertex(v3);
		st->set_uv(uv4);
		st->add_vertex(v4);
		st->set_uv(uv5);
		st->add_vertex(v5);
	}

	for (int seg_idx = 0; seg_idx < num_segs - 1; seg_idx++) {
		int seg_off = seg_idx * 6;
		st->add_index(seg_off + 0);
		st->add_index(seg_off + 1);
		st->add_index(seg_off + 2);

		st->add_index(seg_off + 2);
		st->add_index(seg_off + 1);
		st->add_index(seg_off + 3);

		st->add_index(seg_off + 2);
		st->add_index(seg_off + 3);
		st->add_index(seg_off + 4);

		st->add_index(seg_off + 4);
		st->add_index(seg_off + 3);
		st->add_index(seg_off + 5);
	}

	st->generate_normals();
	set_mesh(st->commit());
}

} //namespace godot