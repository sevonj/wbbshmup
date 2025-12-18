#pragma once

#include <godot_cpp/classes/label.hpp>

namespace godot {

class UiHudScoreNotifMsg : public Label {
	GDCLASS(UiHudScoreNotifMsg, Label)

	static constexpr double LIFETIME = 3.0;

	double t_alive = 0.0;

protected:
	static void _bind_methods();

public:
	UiHudScoreNotifMsg() = default;
	~UiHudScoreNotifMsg() override = default;

	void _ready() override;
	void _process(double delta) override;

private:
	void setup_layout();
};

} //namespace godot