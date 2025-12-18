#pragma once

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/v_box_container.hpp>

namespace godot {

class UiHudScoreNotifCont : public VBoxContainer {
	GDCLASS(UiHudScoreNotifCont, VBoxContainer)

protected:
	static void _bind_methods();

public:
	UiHudScoreNotifCont() = default;
	~UiHudScoreNotifCont() override = default;

	void _ready() override;

private:
	void notify(String text);
};

} //namespace godot