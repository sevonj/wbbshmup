#pragma once

#include <godot_cpp/classes/curve3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/path3d.hpp>

namespace godot {

/// @brief Simple animation tool. Makes a node move along a path. Parent must be a Path3D.
class AnimPathFollow : public Node3D {
	GDCLASS(AnimPathFollow, Node3D)

	bool autostart = false;
	bool oneshot = true;
	bool use_rotation = true;
	bool run_in_editor = false;
	bool running = false;
	float speed = 1.;
	double progress = 0.;

	Ref<Curve3D> get_curve();

protected:
	static void _bind_methods();

public:
	AnimPathFollow() = default;
	~AnimPathFollow() override = default;

	bool get_autostart() { return autostart; };
	void set_autostart(bool v) { autostart = v; };
	bool get_oneshot() { return oneshot; };
	void set_oneshot(bool v) { oneshot = v; };
	bool get_use_rotation() { return use_rotation; };
	void set_use_rotation(bool v) { use_rotation = v; };
	bool get_run_in_editor() { return run_in_editor; };
	void set_run_in_editor(bool v) { run_in_editor = v; };
	bool get_running() { return running; };
	void set_running(bool v) { running = v; };
	float get_speed() { return speed; };
	void set_speed(float v) { speed = v; };
	double get_progress() { return progress; };
	void set_progress(double v) { progress = v; };
	double get_progress_ratio() {
		Ref<Curve3D> curve = get_curve();
		if (!curve.is_valid()) {
			return 0.;
		}
		return progress / curve->get_baked_length();
	}
	void set_progress_ratio(double value) {
		Ref<Curve3D> curve = get_curve();
		if (!curve.is_valid()) {
			return;
		}
		progress = value * curve->get_baked_length();
	}

	virtual void _ready() override;
	virtual void _process(double delta) override;

	void play();
	void stop();
	void pause();
	void restart();
};

} //namespace godot