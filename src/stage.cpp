#include "stage.h"

//#include <entities/camera_rig_tp.h>
#include <entities/player.h>
#include <game.h>
#include <ui/ui_debug_wbbstate.h>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

namespace godot {

void Stage::_bind_methods() {}

Stage::Stage() {
	stage_bounds = DEFAULT_STAGE_BOUNDS;
	local_env = nullptr;
	local_entities = nullptr;
	local_static = nullptr;
	local_ui = nullptr;

	Game::set_stage(this);
}

Stage::~Stage() {}

void Stage::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	local_env = get_node<Node>("env");
	local_entities = get_node<Node>("entities");
	local_static = get_node<Node>("static");
	local_ui = get_node<Node>("ui");

	//find_player_starts();

	spawn_player();
	// toast("real yakuza use a balance board"); TODO
	add_ui(memnew(UiDebugWbbstate));
}

void Stage::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (Input::get_singleton()->is_key_pressed(KEY_F8)) {
		get_tree()->quit();
	}
}

void Stage::add_entity(Node3D *ent) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	local_entities->add_child(ent);
}

void Stage::add_ui(Control *ui) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	local_ui->add_child(ui);
}

// void Stage::find_player_starts() {
// 	TypedArray<Node> stack;
// 	stack.push_back(local_entities);
// 	while (!stack.is_empty()) {
// 		Node *child = Object::cast_to<Node>(stack.pop_front());
// 		InfoPlayerStart *start = cast_to<InfoPlayerStart>(child);
// 		if (start) {
// 			player_starts.push_back(start);
// 		}
// 		stack.append_array(child->get_children());
// 	}
// 	if (player_starts.is_empty()) {
// 		print_error("Stage has no InfoPlayerStart! Spawning player at 0,0,0 instead.");
// 	}
// }

void Stage::spawn_player() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	clear_player();

	Player *player = memnew(Player);
	//CameraRigTp *camera = memnew(CameraRigTp);

	player->connect("died", callable_mp(this, &Stage::on_player_death));
	add_entity(player);

	//if (!player_starts.is_empty()) {
	//	player->set_global_transform(player_starts[0]->get_global_transform());
	//}

	//add_entity(camera);
	//camera->set_global_transform(player->get_global_transform());
}

void Stage::clear_player() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Player *player = Game::get_player();
	//CameraRig *camera = Game::get_current_camera();
	if (player) {
		player->queue_free();
	}
	//if (camera) {
	//	camera->queue_free();
	//}
}

void Stage::on_player_death() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	get_tree()->create_timer(3.)->connect("timeout", callable_mp(this, &Stage::spawn_player));
}

} //namespace godot