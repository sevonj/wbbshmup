#pragma once

#include <entities/enm_rhino.h>
#include <godot_cpp/classes/texture_rect.hpp>

namespace godot {

class UiHudRhinoAlert : public TextureRect {
	GDCLASS(UiHudRhinoAlert, TextureRect)

private:
	const String TEX_PATH = "res://assets/ui/tex_hud_rhino_alert_offscreen.png";

	EnmRhino *target = nullptr;

	void setup_layout();

protected:
	static void _bind_methods();

public:
	UiHudRhinoAlert();
	~UiHudRhinoAlert();

	void _ready() override;
	void _process(double delta) override;

	void set_target(EnmRhino *value);
};

} //namespace godot