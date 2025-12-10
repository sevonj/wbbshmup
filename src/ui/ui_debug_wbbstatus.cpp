#include "ui_debug_wbbstatus.h"

#include <singleton/debug_draw.h>

namespace godot {

void UiDebugWbbstatus::_bind_methods() {
}

UiDebugWbbstatus::UiDebugWbbstatus() {
	label = nullptr;
	axis_box = nullptr;
}

UiDebugWbbstatus::~UiDebugWbbstatus() {
}

void UiDebugWbbstatus::_ready() {
	wbb_input = WbbInput::get_singleton();

	setup_layout();
}

void UiDebugWbbstatus::_process(double delta) {
	if (wbb_input->get_balance_board()) {
		label->set_text("WBB: Connected");
	} else {
		label->set_text("WBB: Not connected. [P] to connect.");
	}

	debug_draw_axis();
}

void UiDebugWbbstatus::setup_layout() {
	label = memnew(Label);
	add_child(label);

	Control *axis_box_cont = memnew(Control);
	add_child(axis_box_cont);

	axis_box = memnew(ColorRect);
	axis_box->set_custom_minimum_size(Vector2(AXIS_BOX_SIZE, AXIS_BOX_SIZE));
	axis_box->set_color(Color::from_hsv(0., 0., 0., 0.5));
	axis_box_cont->add_child(axis_box);
	axis_box->set_position(Vector2(16., 32.));
}

void UiDebugWbbstatus::debug_draw_axis() {
	Vector2 box_min = axis_box->get_position();
	Vector2 box_center = Vector2(box_min.x + AXIS_BOX_SIZE / 2., box_min.y + AXIS_BOX_SIZE / 2.);

	Color gray = Color::from_hsv(0., 0., 0.5, 1.);
	Color white = Color::from_hsv(0., 0., 1., 1.);

	Vector2 h_a = Vector2(box_min.x, box_min.y + AXIS_BOX_SIZE / 2.);
	Vector2 h_b = Vector2(box_min.x + AXIS_BOX_SIZE, box_min.y + AXIS_BOX_SIZE / 2.);
	Vector2 v_a = Vector2(box_min.x + AXIS_BOX_SIZE / 2., box_min.y);
	Vector2 v_b = Vector2(box_min.x + AXIS_BOX_SIZE / 2., box_min.y + AXIS_BOX_SIZE);

	DebugDraw::draw_line_2d(h_a, h_b, gray);
	DebugDraw::draw_line_2d(v_a, v_b, gray);

	Vector2 axis_point = wbb_input->get_axis() * AXIS_BOX_SIZE / 2. + box_center;
	DebugDraw::draw_point_2d(axis_point, white);
}

} //namespace godot