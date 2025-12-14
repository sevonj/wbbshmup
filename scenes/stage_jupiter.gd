extends Stage

@onready var intro_cutscn: Cutscene = $entities/intro_cutscene

func _ready() -> void:
	
	intro_cutscn.fin.connect(_start_game)
	intro_cutscn.play()

func _start_game() -> void:
	spawn_player()
	add_ui(UiStageBeginScreen.new())
