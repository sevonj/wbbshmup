#pragma once

//#include <entities/info_player_start.h>
#include <consts.h>
#include <entities/player.h>
#include <entities/stage_path_grid.h>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/marker3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/path3d.hpp>
#include <godot_cpp/variant/aabb.hpp>

namespace godot {

class Stage : public Node3D {
	GDCLASS(Stage, Node3D)

	static constexpr double INTRO_SCREEN_DURATION = 5.0;
	static constexpr double DEFAULT_RAIL_SPEED = 10.0;

	bool is_playing = false;
	double rail_follow_offset = 0.;
	double rail_speed = DEFAULT_RAIL_SPEED;
	double intro_screen_timer = INTRO_SCREEN_DURATION;

	AABB stage_bounds = DEFAULT_STAGE_BOUNDS;
	Node *local_env = nullptr;
	Node *local_entities = nullptr;
	Node *local_static = nullptr;
	Node *local_ui = nullptr;

	/// @brief Path for the on-rail gameplay.
	Path3D *rail_path = nullptr;
	/// @brief Parent of player. Is moved along the path.
	Marker3D *rail_follow = nullptr;
	StagePathGrid *rail_grid = nullptr;

protected:
	static void _bind_methods();
	void _notification(int what);

public:
	Stage();
	~Stage() override;

	void _ready() override;
	// _process() needs to exist in order to receive NOTIFICATION_PROCESS.
	// The dummy is here in case the stage script doesn't exist or have it.
	void _process(double delta) override {};

	void add_entity(Node3D *ent);
	void add_ui(Control *ui);

	Transform3D sample_rail_start();
	Transform3D sample_rail_end();
	Transform3D sample_rail_at_time(double time);

private:
	Player *_get_player();

	void spawn_player();
	void clear_player();

	void on_player_death();

	void ensure_nodes();
	void ensure_rail_path();
	void ensure_rail_grid();

	void tool_rebuild_grid();
};

} //namespace godot