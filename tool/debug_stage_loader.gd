class_name DebugStageLoader
extends Window
## This creates debug menu for loading worlds.
##
## It will recursively search the scene directory for scenes.
##

# Parent of all list items
var _vbox := VBoxContainer.new()


static func _create_list_item(scene_path: String, tree: SceneTree) -> Label:
	var item := Label.new()
	item.text = scene_path
	item.mouse_filter = Control.MOUSE_FILTER_STOP

	# Change level if event is clicked
	var on_item_input := func(event: InputEvent) -> void:
		if event is InputEventMouseButton:
			var mevent := event as InputEventMouseButton
			if mevent.pressed and mevent.button_index == MOUSE_BUTTON_LEFT:
				print("DebugStageLoader: Loading scene from %s" % scene_path)
				tree.change_scene_to_file(scene_path)
	item.gui_input.connect(on_item_input)

	# Hover effect
	item.mouse_entered.connect(func() -> void: item.modulate = Color.AQUAMARINE)
	item.mouse_exited.connect(func() -> void: item.modulate = Color.WHITE)

	return item


## Recursively search the scenes directory for playable scenes
## (filepath matching "res://scenes/**/stage_*.tscn")
static func get_stages(path: String = "res://scenes") -> PackedStringArray:
	var scenelist := PackedStringArray()
	var dir := DirAccess.open(path)
	if dir:
		dir.list_dir_begin()
		var filename := dir.get_next()
		while filename != "":
			if dir.current_is_dir():
				scenelist += get_stages(path + "/" + filename)
			else:
				if filename.begins_with("stage_") and filename.ends_with(".tscn"):
					scenelist.append(path + "/" + filename)
			filename = dir.get_next()
	else:
		print("Globals::get_stages(): Tried to open a file!")
	return scenelist


func _init() -> void:
	title = "DebugStageLoader"
	position = Vector2(128, 128)
	size = Vector2(640.0, 480.0)
	add_theme_constant_override("separation", 0)


func _ready() -> void:
	var margin := MarginContainer.new()
	var bg := ColorRect.new()
	var scroll := ScrollContainer.new()

	margin.set_anchors_preset(Control.PRESET_FULL_RECT)
	bg.color = Color.BLACK

	add_child(margin)
	margin.add_child(bg)
	margin.add_child(scroll)
	scroll.add_child(_vbox)

	refresh()


## Rebuilds the menu
func refresh() -> void:
	for item in _vbox.get_children():
		item.queue_free()

	var scenes := get_stages()
	scenes.sort()

	var tree := get_tree()
	for scene_path in scenes:
		_vbox.add_child(_create_list_item(scene_path, tree))
