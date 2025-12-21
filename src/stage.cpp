#include "stage.h"

//#include <entities/camera_rig_tp.h>
#include <entities/camera_rig_follow.h>
#include <game.h>
#include <ui/ui_debug_wbbstatus.h>
#include <ui/ui_stage_begin_screen.h>
#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>

namespace godot {

void Stage::_bind_methods() {
	ClassDB::bind_method(D_METHOD("_get_player"), &Stage::_get_player);
	ClassDB::bind_method(D_METHOD("_tool_rebuild_grid"), &Stage::tool_rebuild_grid);
	ClassDB::bind_method(D_METHOD("sample_rail_at_time"), &Stage::sample_rail_at_time);
	ClassDB::bind_method(D_METHOD("sample_rail_at_gameplay_time"), &Stage::sample_rail_at_gameplay_time);
	ClassDB::bind_method(D_METHOD("sample_rail_at_gameplay_start"), &Stage::sample_rail_at_gameplay_start);
	ClassDB::bind_method(D_METHOD("sample_rail_at_gameplay_end"), &Stage::sample_rail_at_gameplay_end);
	ClassDB::bind_method(D_METHOD("spawn_player"), &Stage::spawn_player);
	ClassDB::bind_method(D_METHOD("add_entity"), &Stage::add_entity);
	ClassDB::bind_method(D_METHOD("add_ui"), &Stage::add_ui);

	ADD_SIGNAL(MethodInfo("score_added", PropertyInfo(Variant::INT, "delta")));
	ADD_SIGNAL(MethodInfo("score_notify", PropertyInfo(Variant::STRING, "text")));
}

void Stage::_notification(int what) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	switch (what) {
		case NOTIFICATION_READY:
			ensure_nodes();
			break;

		case NOTIFICATION_PROCESS: {
			if (Input::get_singleton()->is_key_pressed(KEY_F8)) {
				get_tree()->quit();
				return;
			}
			if (Input::get_singleton()->is_key_pressed(KEY_R)) {
				get_tree()->reload_current_scene();
				return;
			}

			double delta = get_process_delta_time();
			if (is_playing) {
				intro_screen_timer -= delta;
			}
			bool intro_wait = intro_screen_timer > 0.;

			Player *player = Game::get_player();
			if (player) {
				player->set_input_enabled(!intro_wait);
			}
		} break;

		default:
			break;
	}
}

Stage::Stage() {
	Game::set_stage(this);
}

Stage::~Stage() {
	Game::set_stage(nullptr);
}

void Stage::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		ensure_rail_grid();
		return;
	}

	//find_player_starts();

	spawn_player();
	// toast("real yakuza use a balance board"); TODO
	add_ui(memnew(UiDebugWbbstatus));
	add_ui(memnew(UiStageBeginScreen));
}

void Stage::add_entity(Node3D *ent) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	ensure_nodes();

	local_entities->add_child(ent);
}

void Stage::add_ui(Control *ui) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	ensure_nodes();

	local_ui->add_child(ui);
}

void Stage::add_score(int64_t v) {
	v *= score_mult;
	score += v;
	print_line("score: +", v, " total: ", score);
	emit_signal("score_added", v);
}

void Stage::mark_clean_segment() {
	emit_signal("score_notify", "Clean Section");
	score_mult += 1;
}

void Stage::mark_strike() {
	emit_signal("score_notify", "STRIKE!");
	score_mult += 1;
}

void Stage::reset() {
	spawn_player();
	score = 0;
	score_mult = 1;
}

Transform3D Stage::sample_rail_at_time(double time) {
	ensure_rail_path();
	double offset = time * rail_speed;
	Transform3D xform = rail_path->get_curve()->sample_baked_with_rotation(offset);
	xform.origin += rail_path->get_global_position();
	return xform;
}

Transform3D Stage::sample_rail_at_gameplay_time(double time) {
	time += INTRO_SCREEN_DURATION;
	ensure_rail_path();
	double offset = time * rail_speed;
	Transform3D xform = rail_path->get_curve()->sample_baked_with_rotation(offset);
	xform.origin += rail_path->get_global_position();
	return xform;
}

Transform3D Stage::sample_rail_at_gameplay_start() {
	ensure_rail_path();
	double start_off = DEFAULT_RAIL_SPEED * INTRO_SCREEN_DURATION;
	Transform3D xform = rail_path->get_curve()->sample_baked_with_rotation(start_off);
	xform.origin += rail_path->get_global_position();
	return xform;
}

Transform3D Stage::sample_rail_at_gameplay_end() {
	ensure_rail_path();
	double end_off = rail_path->get_curve()->get_baked_length();
	Transform3D xform = rail_path->get_curve()->sample_baked_with_rotation(end_off);
	xform.origin += rail_path->get_global_position();
	return xform;
}

Player *Stage::_get_player() {
	return Game::get_player();
}

void Stage::spawn_player() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	ensure_nodes();

	clear_player();

	Player *player = memnew(Player);
	CameraRigFollow *camera = memnew(CameraRigFollow);

	player->connect("died", callable_mp(this, &Stage::on_player_death));
	player->connect("damaged", callable_mp(this, &Stage::on_player_damaged));
	add_entity(player);

	add_entity(camera);
	camera->get_camera()->make_current();
	is_playing = true;

	Ref<PackedScene> hud_res = ResourceLoader::get_singleton()->load(HUD_PATH, "PackedScene");
	if (!hud_res.is_valid()) {
		print_error("Player hud resource missing.");
		return;
	}
	Node *hud_node = hud_res->instantiate();
	Control *hud = cast_to<Control>(hud_node);
	if (hud) {
		add_ui(hud);
	} else {
		print_error("Player hud wasn't a Control");
		memdelete(hud_node);
	}
}

void Stage::clear_player() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Player *player = Game::get_player();
	if (player) {
		player->queue_free();
	}

	CameraRig *camera = Game::get_current_camera();
	if (camera) {
		camera->queue_free();
	}

	if (hud) {
		hud->queue_free();
		hud = nullptr;
	}
}

void Stage::on_player_death() {
	get_tree()->create_timer(PLAYER_DEATH_WAIT_DURATION)->connect("timeout", callable_mp(this, &Stage::reset));
}

void Stage::on_player_damaged() {
	score_mult = 1;
}

void Stage::ensure_nodes() {
	local_env = get_node<Node>("env");
	local_entities = get_node<Node>("entities");
	local_static = get_node<Node>("static");
	local_ui = get_node<Node>("ui");

	rail_path = get_node<Path3D>("rail_path");
}

void Stage::ensure_rail_path() {
	Node *child = get_node_or_null("rail_path");
	rail_path = cast_to<Path3D>(child);
	if (!rail_path) {
		if (child) {
			// Correctly named node exists but it's not a Path3D!
			child->set_name("rail_path_wtf");
		}
		rail_path = memnew(Path3D);
		rail_path->set_name("rail_path");
		add_child(rail_path);
		rail_path->set_owner(get_tree()->get_edited_scene_root());
	}

	if (!rail_path->get_curve().is_valid()) {
		rail_path->set_curve(memnew(Curve3D));
	}
	Ref<Curve3D> curve = rail_path->get_curve();
	while (curve->get_point_count() < 2) {
		curve->add_point(Vector3(0., 0., 10.) * curve->get_point_count());
	}
}

void Stage::ensure_rail_grid() {
	ensure_rail_path();

	Node *child = get_node_or_null("rail_grid");
	rail_grid = cast_to<StagePathGrid>(child);
	if (!rail_grid) {
		if (child) {
			// Correctly named node exists but it's not a StagePathGrid!
			child->set_name("rail_grid_wtf");
		}
		rail_grid = memnew(StagePathGrid);
		rail_grid->set_name("rail_grid");
		add_child(rail_grid);
		rail_grid->set_owner(get_tree()->get_edited_scene_root());
	}
}

void Stage::tool_rebuild_grid() {
	ensure_rail_grid();
	rail_grid->rebuild_mesh(rail_path->get_curve());
	rail_grid->set_transform(rail_path->get_transform());
}

} //namespace godot