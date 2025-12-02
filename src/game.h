#pragma once

#include <entities/camera_rig.h>
#include <entities/player.h>
//#include <stage.h>
#include <godot_cpp/variant/aabb.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot {

const AABB DEFAULT_STAGE_BOUNDS = AABB(
		Vector3(-512, -64, -512),
		Vector3(1024, 512, 1024));

class Game {
private:
	static Game *singleton;

	static Vector<CameraRig *> cameras;
	static CameraRig *current_camera;
	static Player *player;
	//static Stage *stage;

	Game();

protected:
public:
	static Game *get_singleton();

	static CameraRig *get_current_camera();
	static void set_current_camera(CameraRig *camera);
	static Player *get_player();
	static void set_player(Player *value);
	//static Stage *get_stage();
	//static void set_stage(Stage *value);

	static bool is_out_of_bounds(Vector3 node);
	static bool is_out_of_bounds(Node3D *node);

	static bool is_editor();
};

} //namespace godot