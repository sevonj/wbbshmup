#include "cutscene.h"

#include <game.h>
#include <godot_cpp/classes/input.hpp>

namespace godot {

void Cutscene::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_playing"), &Cutscene::get_duration);
	ClassDB::bind_method(D_METHOD("set_playing"), &Cutscene::set_duration);
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing"), "set_playing", "get_playing");
	ClassDB::bind_method(D_METHOD("get_duration"), &Cutscene::get_duration);
	ClassDB::bind_method(D_METHOD("set_duration"), &Cutscene::set_duration);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "duration"), "set_duration", "get_duration");
	ClassDB::bind_method(D_METHOD("get_time"), &Cutscene::get_time);
	ClassDB::bind_method(D_METHOD("set_time"), &Cutscene::set_time);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");

	ClassDB::bind_method(D_METHOD("play"), &Cutscene::play);
	ClassDB::bind_method(D_METHOD("stop"), &Cutscene::stop);

	ADD_SIGNAL(MethodInfo("fin"));
}

void Cutscene::_notification(int what) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	switch (what) {
		case NOTIFICATION_PROCESS:

			if (playing) {
				time += get_process_delta_time();

				if (time > duration || Input::get_singleton()->is_key_pressed(KEY_ESCAPE)) {
					stop();
				}
			}
			set_visible(playing);
			break;

		default:
			break;
	}
}

void Cutscene::play() {
	time = 0.0;
	playing = true;
}

void Cutscene::stop() {
	playing = false;
	emit_signal("fin");
}

} //namespace godot