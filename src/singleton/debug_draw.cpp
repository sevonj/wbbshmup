#include "debug_draw.h"
#include <game.h>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

DebugDraw *DebugDraw::singleton = nullptr;

void DebugDraw::_bind_methods() {
	ClassDB::bind_static_method(get_class_static(), D_METHOD("draw_line_3d"), &DebugDraw::draw_line_3d);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("draw_line_2d"), &DebugDraw::draw_line_2d);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("draw_point_3d"), &DebugDraw::draw_point_3d);
	ClassDB::bind_static_method(get_class_static(), D_METHOD("draw_point_2d"), &DebugDraw::draw_point_2d);
}

DebugDraw *DebugDraw::get_singleton() {
	if (unlikely(singleton == nullptr)) {
		singleton = memnew(DebugDraw);
		SceneTree *scene_tree = (SceneTree *)Engine::get_singleton()->get_main_loop();
		scene_tree->get_current_scene()->add_child(singleton);
	}
	return singleton;
}

DebugDraw::DebugDraw() {
	CRASH_COND(singleton != nullptr);
	singleton = this;
}

DebugDraw::~DebugDraw() {
	CRASH_COND(singleton == nullptr);
	singleton = nullptr;
}

void DebugDraw::_ready() {
	font = ResourceLoader::get_singleton()->load(FONT_PATH, "Font");
}

void DebugDraw::_process(double delta) {
	queue_redraw();
}

void DebugDraw::_draw() {
	if (!Game::get_singleton()->get_current_camera()) {
		draw_text("DebugDraw: No Camera!");
		clear_queues();
		return;
	}

	for (auto l : line_queue) {
		draw_line(l.a, l.b, l.color);
	}

	for (auto p : point_queue) {
		auto rect = Rect2(Vector2(p.position.x - 4, p.position.y - 4), Vector2(8, 8));
		draw_rect(rect, p.color, false);
	}

	clear_queues();
}

void DebugDraw::clear_queues() {
	point_queue.clear();
	line_queue.clear();
}

void DebugDraw::draw_line_3d(Vector3 a, Vector3 b, Color color) {
	CameraRig *camera_rig = Game::get_current_camera();
	if (!camera_rig) {
		return;
	}
	Camera3D *camera = camera_rig->get_camera();
	if (camera->is_position_behind(a) || camera->is_position_behind(b)) {
		return;
	}
	DebugDraw *ddraw = get_singleton();
	ddraw->line_queue.push_back(QueueLine{ camera->unproject_position(a), camera->unproject_position(b), color });
}

void DebugDraw::draw_line_2d(Vector2 a, Vector2 b, Color color) {
	DebugDraw *ddraw = get_singleton();
	ddraw->line_queue.push_back(QueueLine{ a, b, color });
}

void DebugDraw::draw_point_3d(Vector3 a, Color color) {
	CameraRig *camera_rig = Game::get_current_camera();
	if (!camera_rig) {
		return;
	}
	Camera3D *camera = camera_rig->get_camera();
	if (camera->is_position_behind(a)) {
		return;
	}
	DebugDraw *ddraw = get_singleton();
	ddraw->point_queue.push_back(QueuePoint{ camera->unproject_position(a), color });
}

void DebugDraw::draw_point_2d(Vector2 a, Color color) {
	DebugDraw *ddraw = get_singleton();
	ddraw->point_queue.push_back(QueuePoint{ a, color });
}

void DebugDraw::draw_text(String text) {
	float x = 2.0;
	char cc[2] = { 0, 0 };
	for (char c : text.to_utf8_buffer()) {
		cc[0] = c;
		draw_char(font, Vector2(x, 200), cc);
		x += 16.0;
	}
}

} //namespace godot