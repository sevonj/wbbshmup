#pragma once

#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/node2d.hpp>

namespace godot {

class DebugDraw : public Node2D {
	GDCLASS(DebugDraw, Node2D)

	struct QueuePoint {
		Vector2 position;
		Color color;
	};

	struct QueueLine {
		Vector2 a;
		Vector2 b;
		Color color;
	};

	struct QueueCircle {
		Vector2 position;
		float r;
		Color color;
	};

	static constexpr const char *FONT_PATH = "res://assets/font/font_debug.tres";

	static DebugDraw *singleton;
	Ref<Font> font = Ref<Font>();

	Vector<QueuePoint> point_queue;
	Vector<QueueLine> line_queue;
	Vector<QueueCircle> circle_queue;

protected:
	static void _bind_methods();

public:
	static DebugDraw *get_singleton();

	static void draw_line_3d(Vector3 a, Vector3 b, Color color);
	static void draw_line_2d(Vector2 a, Vector2 b, Color color);
	static void draw_point_3d(Vector3 origin, Color color);
	static void draw_point_2d(Vector2 position, Color color);
	static void draw_sphere3d(Vector3 origin, float r, Color color);
	static void draw_circle2d(Vector2 position, float r, Color color);

	void _ready() override;
	void _process(double delta) override;
	void _draw() override;

private:
	DebugDraw();
	~DebugDraw() override;

	void draw_text(String text);
	void clear_queues();
};

} //namespace godot
