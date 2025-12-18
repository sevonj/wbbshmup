#include "stage_segment.h"

#include <entities/enemy.h>
#include <entities/player.h>
#include <game.h>
#include <stage.h>
#include <godot_cpp/classes/engine.hpp>

namespace godot {
void StageSegment::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_trigger_start"), &StageSegment::set_trigger_start);
	ClassDB::bind_method(D_METHOD("get_trigger_start"), &StageSegment::get_trigger_start);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trigger_start", PROPERTY_HINT_NODE_TYPE), "set_trigger_start", "get_trigger_start");
	ClassDB::bind_method(D_METHOD("set_trigger_end"), &StageSegment::set_trigger_end);
	ClassDB::bind_method(D_METHOD("get_trigger_end"), &StageSegment::get_trigger_end);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "trigger_end", PROPERTY_HINT_NODE_TYPE), "set_trigger_end", "get_trigger_end");
}

void StageSegment::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	setup_triggers();
	setup_enemies();
}

void StageSegment::setup_triggers() {
	if (trigger_start) {
		trigger_start->connect("triggered", callable_mp(this, &StageSegment::start_segment));
	} else {
		print_error("StageSegment: No start trigger!");
	}
	if (trigger_end) {
		trigger_end->connect("triggered", callable_mp(this, &StageSegment::end_segment));
	} else {
		print_error("StageSegment: No end trigger!");
	}
}

void StageSegment::setup_enemies() {
	TypedArray<Node> nodes = TypedArray<Node>();
	nodes.append_array(get_children());

	while (!nodes.is_empty()) {
		Node *node = cast_to<Node>(nodes.pop_front());
		nodes.append_array(node->get_children());

		Enemy *enemy = cast_to<Enemy>(node);
		if (enemy && enemy->counts_towards_strike()) {
			num_enemies += 1;
			enemy->connect("died", callable_mp(this, &StageSegment::on_enemy_killed));
		}
	}
	print_line("found '", num_enemies, "' enemies.");
}

void StageSegment::start_segment() {
	active = true;
	Player *player = Game::get_player();
	if (!player) {
		print_line("start_segment(): wtf, no player");
		return;
	}
	player->connect("damaged", callable_mp(this, &StageSegment::on_player_damaged));
	print_line("Segment started: '", get_name(), "'");
}

void StageSegment::end_segment() {
	if (!active) {
		return;
	}
	active = false;
	print_line("Segment ended: '", get_name(), "'");
	print_line("enemies killed: '", num_enemies_killed, "'/'", num_enemies, "'");

	if (!clean_failed) {
		Game::get_stage()->mark_clean_segment();
		print_line("Clean section");
	}
}

void StageSegment::on_player_damaged() {
	print_line("seg: dmgd");
	if (!clean_failed) {
		print_line("Clean section failed");
	}
	clean_failed = true;
}

void StageSegment::on_enemy_killed() {
	num_enemies_killed += 1;
	if (num_enemies_killed == num_enemies) {
		Game::get_stage()->mark_strike();
		print_line("Strike!");
	}
}

} //namespace godot