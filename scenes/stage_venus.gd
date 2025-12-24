extends Stage

func _ready() -> void:
	_start_game()

func _start_game() -> void:
	spawn_player()
	var begin_screen := UiStageBeginScreen.new()
	begin_screen.set_stage_no("Stage 02")
	begin_screen.set_stage_name("Dome Jungle")
	begin_screen.set_stage_desc("Venus Tropical Dome")
	begin_screen.set_stage_icon(preload("res://assets/ui/stage_icons/tex_icon_stage_venus.png"))
	add_ui(begin_screen)
	add_ui(UiDebugWbbstatus.new())
