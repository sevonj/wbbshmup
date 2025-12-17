#include "tool_stage_path_marker.h"

#include <godot_cpp/classes/engine.hpp>

namespace godot {

void ToolStagePathMarker::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_time"), &ToolStagePathMarker::get_time);
	ClassDB::bind_method(D_METHOD("set_time"), &ToolStagePathMarker::set_time);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "time"), "set_time", "get_time");
}

void ToolStagePathMarker::_process(double delta) {
	if (!Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	Stage *stage = cast_to<Stage>(get_parent());
	if (!stage) {
		return;
	}
	set_transform(stage->sample_rail_at_gameplay_time(time));
}

} //namespace godot