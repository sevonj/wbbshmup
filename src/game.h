#pragma once

#include <entities/camera_rig.h>
#include <entities/player.h>
#include <stage.h>

namespace godot {

class Game {
private:
	static Game *singleton;

	static Vector<CameraRig *> cameras;
	static CameraRig *current_camera;
	static Player *player;
	static Stage *stage;

	Game();

protected:
public:
	static Game *get_singleton();

	static void set_current_camera(CameraRig *v) { current_camera = v; }
	static CameraRig *get_current_camera() { return current_camera; }
	static void set_player(Player *v) { player = v; }
	static Player *get_player() { return player; }
	static void set_stage(Stage *v) { stage = v; }
	static Stage *get_stage() { return stage; }

	static bool is_out_of_bounds(Vector3 node);
	static bool is_out_of_bounds(Node3D *node) { return is_out_of_bounds(node->get_global_position()); }
};

} //namespace godot