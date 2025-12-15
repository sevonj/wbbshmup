extends EditorInspectorPlugin

const ICON_BAKE = preload("res://addons/sticky_tools/classes/tool/world_nav_bake.png")


func _can_handle(object):
	return object is ToolPathDeformMesh


func _parse_begin(object):
	_add_bake_button(object)


func _add_bake_button(pdm: ToolPathDeformMesh):
	var button := Button.new()
	button.text = "Rebuild Mesh"
	button.icon = ICON_BAKE
	button.expand_icon = true
	button.pressed.connect(pdm.rebuild_mesh)
	add_custom_control(button)
