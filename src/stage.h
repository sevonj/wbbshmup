#pragma once

//#include <entities/info_player_start.h>
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

private:
	static constexpr double INTRO_WAIT_DURATION = 10.;

	AABB stage_bounds;
	Node *local_env;
	Node *local_entities;
	Node *local_static;
	Node *local_ui;

	/// @brief Path for the on-rail gameplay.
	Path3D *rail_path;
	/// @brief Parent of player. Is moved along the path.
	Marker3D *rail_follow;
	/// @brief Offset of rail follower
	double rail_offset;
	StagePathGrid *rail_grid;

	double intro_wait_timer;

	void tool_ensure_rail_path();
	void tool_ensure_rail_grid();
	void tool_rebuild_grid();

	//Vector<InfoPlayerStart *> player_starts;

	//void find_player_starts();
	void spawn_player();
	void clear_player();

	void on_player_death();

protected:
	static void _bind_methods();

public:
	Stage();
	~Stage();

	void _ready() override;
	void _process(double delta) override;

	void add_entity(Node3D *ent);
	void add_ui(Control *ui);
};

} //namespace godot