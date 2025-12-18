#pragma once

#include <entities/func_trigger.h>
#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Emits signal when player enters or exits it
class StageSegment : public Node3D {
	GDCLASS(StageSegment, Node3D)

	FuncTrigger *trigger_start = nullptr;
	FuncTrigger *trigger_end = nullptr;

	bool active = false;
	bool clean_failed = false;
	int32_t num_enemies = 0;
	int32_t num_enemies_killed = 0;

protected:
	static void _bind_methods();

public:
	StageSegment() = default;
	~StageSegment() override = default;

	void set_trigger_start(FuncTrigger *v) { trigger_start = v; }
	FuncTrigger *get_trigger_start() const { return trigger_start; }
	void set_trigger_end(FuncTrigger *v) { trigger_end = v; }
	FuncTrigger *get_trigger_end() const { return trigger_end; }

	void _ready() override;

private:
	void setup_triggers();
	void setup_enemies();

	void start_segment();
	void end_segment();

	void on_player_damaged();
	void on_enemy_killed();
};

} //namespace godot