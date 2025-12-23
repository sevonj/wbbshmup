extends MainMenu

func _ready() -> void:
	var wbb_connect_prompt := UiMenuWbbConnectPrompt.new()
	wbb_connect_prompt.completed.connect(_show_stage_select)
	wbb_connect_prompt.completed.connect(wbb_connect_prompt.queue_free)
	add_child(wbb_connect_prompt)

func _show_stage_select() -> void:
	var stageselect := DebugStageLoader.new()
	add_child(stageselect)
