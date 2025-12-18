#include "ui_hud_score_notif_cont.h"

#include <game.h>
#include <stage.h>
#include <ui/ui_hud_score_notif_msg.h>
#include <godot_cpp/classes/engine.hpp>

namespace godot {

void UiHudScoreNotifCont::_bind_methods() {}

void UiHudScoreNotifCont::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		notify("Clean Section!");
		notify("STRIKE!");
		notify("sample text");
		notify("sample text");
		notify("sample text");
		return;
	}

	Stage *stage = Game::get_stage();
	if (stage) {
		stage->connect("score_notify", callable_mp(this, &UiHudScoreNotifCont::notify));
	}
}

void UiHudScoreNotifCont::notify(String text) {
	UiHudScoreNotifMsg *msg = memnew(UiHudScoreNotifMsg);
	msg->set_text(text);
	add_child(msg);
}

} //namespace godot