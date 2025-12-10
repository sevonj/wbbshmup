extends EditorInspectorPlugin

const ICON_BAKE = preload("res://addons/sticky_tools/classes/tool/world_nav_bake.png")


func _can_handle(object):
	return object is Stage


func _parse_begin(object):
	_add_bake_button(object)


func _add_bake_button(stage: Stage):
	var button := Button.new()
	button.text = "Rebuild Grid"
	button.icon = ICON_BAKE
	button.expand_icon = true
	button.pressed.connect(stage._tool_rebuild_grid)
	add_custom_control(button)
