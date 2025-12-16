extends Stage

@onready var intro_cutscn: Cutscene = $entities/intro_cutscene
@onready var vfx_part_spacedust: Node3D = $entities/vfx_part_spacedust

func _ready() -> void:
	
	intro_cutscn.fin.connect(_start_game)
	intro_cutscn.play()
	

func _start_game() -> void:
	spawn_player()
	add_ui(UiStageBeginScreen.new())
	add_ui(UiDebugWbbstatus.new())

func _process(_delta: float) -> void:
	var player := _get_player()
	if is_instance_valid(player):
		vfx_part_spacedust.global_position = player.global_position - player.global_basis.z * 50.0
