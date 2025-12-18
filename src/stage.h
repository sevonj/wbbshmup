#pragma once

//#include <entities/info_player_start.h>
#include <consts.h>
#include <entities/player.h>
#include <entities/stage_path_grid.h>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/path3d.hpp>
#include <godot_cpp/variant/aabb.hpp>

namespace godot {

class Stage : public Node3D {
	GDCLASS(Stage, Node3D)

	static constexpr const char *HUD_PATH = "res://assets/ui/ui_hud.tscn";

	static constexpr double INTRO_SCREEN_DURATION = 5.0;
	static constexpr double PLAYER_DEATH_WAIT_DURATION = 3.0;
	static constexpr double DEFAULT_RAIL_SPEED = 10.0;

	// --- state
	bool is_playing = false;
	double rail_speed = DEFAULT_RAIL_SPEED;
	double intro_screen_timer = INTRO_SCREEN_DURATION;
	int64_t score = 0;
	int64_t score_mult = 1;
	AABB stage_bounds = DEFAULT_STAGE_BOUNDS;

	// --- components
	Node *local_env = nullptr;
	Node *local_entities = nullptr;
	Node *local_static = nullptr;
	Node *local_ui = nullptr;
	Path3D *rail_path = nullptr;
	StagePathGrid *rail_grid = nullptr;
	Control *hud = nullptr;

protected:
	static void _bind_methods();
	void _notification(int what);

public:
	Stage();
	~Stage() override;

	void set_score(int64_t v) { score = v; }
	int64_t get_score() { return score; }
	void set_score_mult(int64_t v) { score_mult = v; }
	int64_t get_score_mult() { return score_mult; }

	void _ready() override;
	// _process() needs to exist in order to receive NOTIFICATION_PROCESS.
	// The dummy is here in case the stage script doesn't exist or have it.
	void _process(double delta) override {};

	void add_entity(Node3D *ent);
	void add_ui(Control *ui);

	void add_score(int64_t v);
	void mark_clean_segment();
	void mark_strike();
	void reset();

	Transform3D sample_rail_at_time(double time);
	Transform3D sample_rail_at_gameplay_time(double time);
	Transform3D sample_rail_at_gameplay_start();
	Transform3D sample_rail_at_gameplay_end();

private:
	Player *_get_player();

	void spawn_player();
	void clear_player();

	void on_player_death();
	void on_player_damaged();

	void ensure_nodes();
	void ensure_rail_path();
	void ensure_rail_grid();

	void tool_rebuild_grid();
};

} //namespace godot