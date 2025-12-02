#pragma once

#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/node2d.hpp>

namespace godot {

class DebugDraw : public Node2D {
	GDCLASS(DebugDraw, Node2D)

private:
	const String FONT_PATH = "res://assets/font/font_debug.tres";

	static DebugDraw *singleton;
	Ref<Font> font;

	struct QueuePoint {
		Vector2 position;
		Color color;
	};

	struct QueueLine {
		Vector2 a;
		Vector2 b;
		Color color;
	};

	Vector<QueuePoint> point_queue;
	Vector<QueueLine> line_queue;

	DebugDraw();
	~DebugDraw();

	void clear_queues();
	void draw_text(String text);

protected:
	static void _bind_methods();

public:
	static DebugDraw *get_singleton();

	static void draw_line_3d(Vector3 a, Vector3 b, Color color);
	static void draw_line_2d(Vector2 a, Vector2 b, Color color);
	static void draw_point_3d(Vector3 a, Color color);
	static void draw_point_2d(Vector2 a, Color color);

	void _ready() override;
	void _process(double delta) override;
	void _draw() override;
};

} //namespace godot
