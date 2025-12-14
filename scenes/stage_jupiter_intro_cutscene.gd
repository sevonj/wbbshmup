extends Cutscene

# Shot 1
@onready var flyinto_player_anim: AnimPathFollow = $anim_path_flytostation/follow
@onready var flyinto_cam_anim: AnimPathFollow = $anim_path_flytostationcam/follow
@onready var flyinto_cam: Camera3D = $anim_path_flytostationcam/follow/cam

# Shot 2
@onready var overview_cam_anim: AnimPathFollow = $cam_path_fyinto/follow
@onready var overview_cam: Camera3D = $cam_path_fyinto/follow/cam

const SHOT1_DUR := 14.0
const SHOT2_DUR := 0.0

const SHOT2_START := SHOT1_DUR
const TOTAL := SHOT2_START + SHOT2_DUR

var shot2_played := false

func _init() -> void:
	duration = SHOT1_DUR

func _ready () -> void:
	_play_shot1()

#func _process(_delta: float) -> void:
#	if time > SHOT2_START && !shot2_played:
#		shot2_played = true
#		_play_shot2()

func _play_shot1() -> void:
	flyinto_player_anim.speed *= 1.4
	
	flyinto_player_anim.play()
	flyinto_cam_anim.play()
	flyinto_cam.make_current()

#func _play_shot2() -> void:
#	overview_cam_anim.play()
#	overview_cam.make_current()
	
