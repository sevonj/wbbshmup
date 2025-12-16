extends EditorNode3DGizmoPlugin


func get_name():
	return "Enemy"


func _has_gizmo(node):
	return node is Enemy


func _get_gizmo_name():
	return "Enemy"


func _create_gizmo(for_node_3d: Node3D) -> EditorNode3DGizmo:
	if !for_node_3d is Enemy:
		return null

	var enemy := for_node_3d as Enemy
	return EnemyGizmo.new(enemy)
 
func _redraw(gizmo: EditorNode3DGizmo):
	gizmo = gizmo as EditorNode3DGizmo
	gizmo.clear()
	
	var a := Vector3(1., -1., 0.) 
	var b := Vector3(-1., -1., 0.)
	var c := Vector3(1., 1., 0.)
	var d := Vector3(-1., 1., 0.)
	
	var uv_a := Vector2(1., 1.,)
	var uv_b := Vector2(0., 1.,)
	var uv_c := Vector2(1., 0.,)
	var uv_d := Vector2(0., 0.,)
	
	var st := SurfaceTool.new()
	
	st.begin(Mesh.PRIMITIVE_TRIANGLE_STRIP)
	st.set_material(preload("res://assets/materials/mat_tool_enemy_gizmo.tres"))
	
	st.set_uv(uv_a)
	st.add_vertex( a)
	st.set_uv(uv_b)
	st.add_vertex( b)
	st.set_uv(uv_c)
	st.add_vertex( c)
	st.set_uv(uv_d)
	st.add_vertex( d)
	
	gizmo.add_mesh(st.commit())

class EnemyGizmo:
	extends EditorNode3DGizmo
	
	var enemy: Enemy

	func _init(_enemy: Enemy):
		enemy = _enemy
