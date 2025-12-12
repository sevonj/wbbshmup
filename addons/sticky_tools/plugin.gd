@tool
extends EditorPlugin

#const AUTOLOAD_WE_NAME = "WorldEditor"
#const AUTOLOAD_WE_PATH = "res://addons/worldeditor/autoload/world_editor.gd"
#const AUTOLOAD_GLOBALS_NAME = "WE_Globals"
#const AUTOLOAD_GLOBALS_PATH = "res://addons/worldeditor/autoload/we_globals.gd"
#var editorplugin_we_junction = preload("res://addons/worldeditor/classes/we_intersection/editorplugin.gd").new()
#var editorplugin_we_curve = preload("res://addons/worldeditor/classes/we_spline/editorplugin.gd").new()
#var we_curve_gizmo = preload("res://addons/worldeditor/classes/we_curve/we_curve_gizmo.gd").new()
#var we_curve_inspector = preload("res://addons/worldeditor/classes/we_curve/we_curve_inspector.gd").new()
#var ai_node_gizmo = preload("res://addons/sticky_tools/classes/entities/ai_node/ai_node_gizmo.gd").new()
#var enemy_gizmo = preload("res://addons/sticky_tools/classes/entities/enemy_gizmo.gd").new()
#var info_ent_spawn_point_gizmo = preload("res://addons/sticky_tools/classes/entities/info_ent_spawn_point_gizmo.gd").new()
#var info_player_start_gizmo = (
#	preload("res://addons/sticky_tools/classes/entities/info_player_start_gizmo.gd").new()
#)
#var weapon_boxpunch = preload("res://addons/sticky_tools/classes/entities/weapon_boxpunch/weapon_boxpunch_gizmo.gd").new()
#var info_player_start_inspector = preload("res://addons/worldeditor/classes/we_curve/we_curve_inspector.gd").new()
#var world_inspector = preload("res://addons/sticky_tools/classes/world/world_inspector.gd").new()

var stage_gizmo = preload("res://addons/sticky_tools/classes/stage_gizmo.gd").new()
var stage_inspector = preload("res://addons/sticky_tools/classes/tool/stage_inspector.gd").new()


func _enter_tree():

	# Tool
	add_node_3d_gizmo_plugin(stage_gizmo)
	add_inspector_plugin(stage_inspector)


func _exit_tree():

	# Tool
	remove_node_3d_gizmo_plugin(stage_gizmo)
	remove_inspector_plugin(stage_inspector)
