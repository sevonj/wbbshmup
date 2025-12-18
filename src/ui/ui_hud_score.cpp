#include "ui_hud_score.h"

#include <debug_draw.h>
#include <game.h>
#include <stage.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {

void UiHudScore::_bind_methods() {}

void UiHudScore::_ready() {
	setup_layout();
}

void UiHudScore::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Stage *stage = Game::get_stage();
	if (stage) {
		int64_t score = stage->get_score();
		int64_t mult = stage->get_score_mult();
		lab_score->set_text(String::num_int64(score));
		lab_mult->set_text(String("X") + String::num_int64(mult));
		lab_mult->set_visible(mult != 1);
	}
}

void UiHudScore::setup_layout() {
	VBoxContainer *vbox = memnew(VBoxContainer);
	vbox->set_alignment(BoxContainer::ALIGNMENT_END);
	add_child(vbox);

	lab_score = memnew(Label);
	lab_score->set_text("0");
	lab_score->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
	vbox->add_child(lab_score);

	lab_mult = memnew(Label);
	lab_mult->set_text("X1");
	lab_mult->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
	vbox->add_child(lab_mult);
}

} //namespace godot