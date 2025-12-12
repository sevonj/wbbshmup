extends EditorNode3DGizmoPlugin


func get_name():
	return "Stage"


func _has_gizmo(node):
	return node is Stage


func _get_gizmo_name():
	return "Stage"


func _create_gizmo(for_node_3d: Node3D) -> EditorNode3DGizmo:
	if !for_node_3d is Stage:
		return null

	var stage := for_node_3d as Stage
	return StageGizmo.new(stage)


func _redraw(gizmo: EditorNode3DGizmo):
	gizmo = gizmo as StageGizmo
	gizmo.clear()

	var stage: Stage = gizmo.stage

	
	var path_width := 24.;
	
	var a := Vector3(1., -1., 0.) * path_width
	var b := Vector3(-1., -1., 0.) * path_width
	var c := Vector3(1., 1., 0.) * path_width
	var d := Vector3(-1., 1., 0.) * path_width
	
	var uv_a := Vector2(1., 1.,)
	var uv_b := Vector2(0., 1.,)
	var uv_c := Vector2(1., 0.,)
	var uv_d := Vector2(0., 0.,)
	
	var st := SurfaceTool.new()
	
	st.begin(Mesh.PRIMITIVE_TRIANGLE_STRIP)
	st.set_material(load("res://assets/materials/mat_tool_stage_start.tres"))
	var start_xform := stage.get_stage_start()
	start_xform.origin -= stage.global_position
	
	st.set_uv(uv_a)
	st.add_vertex(start_xform * a)
	st.set_uv(uv_b)
	st.add_vertex(start_xform * b)
	st.set_uv(uv_c)
	st.add_vertex(start_xform * c)
	st.set_uv(uv_d)
	st.add_vertex(start_xform * d)
	
	gizmo.add_mesh(st.commit())
	
	st.begin(Mesh.PRIMITIVE_TRIANGLE_STRIP)
	st.set_material(load("res://assets/materials/mat_tool_stage_end.tres"))
	var end_xform := stage.get_stage_end()
	end_xform.origin -= stage.global_position
	
	st.set_uv(uv_a)
	st.add_vertex(end_xform * a)
	st.set_uv(uv_b)
	st.add_vertex(end_xform * b)
	st.set_uv(uv_c)
	st.add_vertex(end_xform * c)
	st.set_uv(uv_d)
	st.add_vertex(end_xform * d)
	
	gizmo.add_mesh(st.commit())


class StageGizmo:
	extends EditorNode3DGizmo
	
	var stage: Stage

	func _init(_stage: Stage):
		stage = _stage
