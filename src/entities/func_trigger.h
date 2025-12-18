#pragma once

#include <godot_cpp/classes/area3d.hpp>

namespace godot {

/// @brief Emits signal when player enters or exits it
class FuncTrigger : public Area3D {
	GDCLASS(FuncTrigger, Area3D)

	bool enabled = true;
	bool trigger_on_enter = true;
	bool trigger_on_exit = false;
	bool oneshot = false;

protected:
	static void _bind_methods();

public:
	FuncTrigger() = default;
	~FuncTrigger() override = default;

	void set_enabled(bool v) { enabled = v; }
	bool get_enabled() const { return enabled; }
	void set_trigger_on_enter(bool v) { trigger_on_enter = v; }
	bool get_trigger_on_enter() const { return trigger_on_enter; }
	void set_trigger_on_exit(bool v) { trigger_on_exit = v; }
	bool get_trigger_on_exit() const { return trigger_on_exit; }
	void set_oneshot(bool v) { oneshot = v; }
	bool get_oneshot() const { return oneshot; }

	void _ready() override;

private:
	void on_body_entered(Node3D *body);
	void on_body_exited(Node3D *body);
};

} //namespace godot