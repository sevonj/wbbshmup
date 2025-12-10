#include "stage.h"

//#include <entities/camera_rig_tp.h>
#include <entities/camera_rig_follow.h>
#include <entities/player.h>
#include <game.h>
#include <ui/ui_debug_wbbstatus.h>
#include <godot_cpp/classes/curve3d.hpp>
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

	rail = nullptr;
	rail_follow = nullptr;

	rail_offset = 0.;

	Game::set_stage(this);
}

Stage::~Stage() {}

void Stage::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		tool_setup_rail();
		tool_setup_rail_curve();
		return;
	}

	local_env = get_node<Node>("env");
	local_entities = get_node<Node>("entities");
	local_static = get_node<Node>("static");
	local_ui = get_node<Node>("ui");

	rail = get_node<Path3D>("rail");

	//find_player_starts();

	spawn_player();
	// toast("real yakuza use a balance board"); TODO
	add_ui(memnew(UiDebugWbbstatus));
}

void Stage::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (Input::get_singleton()->is_key_pressed(KEY_F8)) {
		get_tree()->quit();
		return;
	}

	rail_offset += delta * 8.;

	if (rail_follow) {
		Transform3D sampled_xform = rail->get_curve()->sample_baked_with_rotation(rail_offset);
		rail_follow->set_transform(sampled_xform);
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

/// @brief TOOL: Makes sure that rail node exists.
void Stage::tool_setup_rail() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Node *child = get_node_or_null("rail");
	if (child) {
		child->set_owner(get_tree()->get_edited_scene_root()); // Probably is already, but justin case

		rail = cast_to<Path3D>(child);
		if (rail) {
			return;
		}
		// Correctly named node exists but it's not a Path3D!
		child->set_name("rail_wtf");
	}

	rail = memnew(Path3D);
	rail->set_name("rail");
	add_child(rail);
	rail->set_owner(get_tree()->get_edited_scene_root());
}

/// @brief TOOL: Makes sure that path's Curve3D is setup.
void Stage::tool_setup_rail_curve() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (rail->get_curve() == nullptr) {
		rail->set_curve(memnew(Curve3D));
	}
	Ref<Curve3D> curve = rail->get_curve();

	while (curve->get_point_count() < 2) {
		curve->add_point(Vector3(0., 0., 10.) * curve->get_point_count());
	}
}

void Stage::spawn_player() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	clear_player();

	Player *player = memnew(Player);
	CameraRigFollow *camera = memnew(CameraRigFollow);

	player->connect("died", callable_mp(this, &Stage::on_player_death));
	rail_offset = 0.;
	rail_follow = memnew(Marker3D);
	rail_follow->add_child(player);
	add_entity(rail_follow);

	//if (!player_starts.is_empty()) {
	//	player->set_global_transform(player_starts[0]->get_global_transform());
	//}

	camera->set_target(rail_follow);
	add_entity(camera);
}

void Stage::clear_player() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (rail_follow) {
		rail_follow->queue_free();
		rail_follow = nullptr;
	}

	//Player *player = Game::get_player();
	CameraRig *camera = Game::get_current_camera();
	//if (player) {
	//	player->queue_free();
	//}
	if (camera) {
		camera->queue_free();
	}
}

void Stage::on_player_death() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	get_tree()->create_timer(3.)->connect("timeout", callable_mp(this, &Stage::spawn_player));
}

} //namespace godot