extends EditorInspectorPlugin

const ICON_BAKE = preload("res://addons/sticky_tools/classes/tool/world_nav_bake.png")


func _can_handle(object):
	return object is ToolPathDeformMesh


func _parse_begin(object):
	var pdm := object as ToolPathDeformMesh
	_add_bake_button(pdm)
	
	_add_label("Start cap: '%s', End cap: '%s'" % [pdm.has_start_cap(), pdm.has_end_cap()])
	_add_label("Piece count: '%d'" % pdm.get_num_pieces())


func _add_bake_button(pdm: ToolPathDeformMesh):
	var button := Button.new()
	button.text = "Rebuild Mesh"
	button.icon = ICON_BAKE
	button.expand_icon = true
	button.pressed.connect(pdm.rebuild_mesh)
	add_custom_control(button)


func _add_label(text: String):
	var lab := Label.new()
	lab.text = text
	add_custom_control(lab)
