extends EditorNode3DGizmoPlugin


func get_name():
	return "ToolStagePathMarker"


func _has_gizmo(node):
	return node is ToolStagePathMarker


func _get_gizmo_name():
	return "ToolStagePathMarker"


func _create_gizmo(for_node_3d: Node3D) -> EditorNode3DGizmo:
	if !for_node_3d is ToolStagePathMarker:
		return null

	var marker := for_node_3d as ToolStagePathMarker
	return ToolStagePathMarkerGizmo.new(marker)
 
func _redraw(gizmo: EditorNode3DGizmo):
	gizmo = gizmo as ToolStagePathMarkerGizmo
	gizmo.clear()

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
	st.set_material(load("res://assets/materials/mat_tool_stage_marker.tres"))
	
	st.set_uv(uv_a)
	st.add_vertex( a)
	st.set_uv(uv_b)
	st.add_vertex( b)
	st.set_uv(uv_c)
	st.add_vertex( c)
	st.set_uv(uv_d)
	st.add_vertex( d)
	
	gizmo.add_mesh(st.commit())

class ToolStagePathMarkerGizmo:
	extends EditorNode3DGizmo
	
	var marker: ToolStagePathMarker

	func _init(_marker: ToolStagePathMarker):
		marker = _marker
