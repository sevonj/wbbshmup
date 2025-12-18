#include "ui_hud_score_notif_msg.h"

#include <debug_draw.h>
#include <game.h>
#include <stage.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {

void UiHudScoreNotifMsg::_bind_methods() {}

void UiHudScoreNotifMsg::_ready() {
	setup_layout();
}

void UiHudScoreNotifMsg::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	t_alive += delta;
	if (t_alive > LIFETIME) {
		queue_free();
	}
}

void UiHudScoreNotifMsg::setup_layout() {
	set_horizontal_alignment(HORIZONTAL_ALIGNMENT_RIGHT);
}

} //namespace godot