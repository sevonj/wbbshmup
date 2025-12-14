#pragma once

#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Container node for a cutscene. Extend this with GDScript.
class Cutscene : public Node3D {
	GDCLASS(Cutscene, Node3D)

private:
	bool playing = false;
	/// @brief Length of the cutscene
	double duration = 5.0;
	/// @brief Playback progress
	double time = 0.0;

protected:
	static void _bind_methods();
	void _notification(int what);

public:
	Cutscene() {};
	~Cutscene() {};

	// _process() needs to exist in order to get NOTIFICATION_PROCESS.
	// This dummy is here in case the script doesn't have it.
	void _process(double delta) override {};

	bool get_playing() { return playing; };
	void set_playing(bool v) { playing = v; };
	double get_duration() { return duration; };
	void set_duration(double v) { duration = v; };
	double get_time() { return time; };
	void set_time(double v) { time = v; };

	void play();
	void stop();
};

} //namespace godot