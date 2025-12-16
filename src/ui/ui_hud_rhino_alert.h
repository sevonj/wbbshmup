#pragma once

#include <entities/enm_rhino.h>
#include <godot_cpp/classes/texture_rect.hpp>

namespace godot {

class UiHudRhinoAlert : public TextureRect {
	GDCLASS(UiHudRhinoAlert, TextureRect)

	static constexpr const char *TEX_PATH = "res://assets/ui/tex_hud_rhino_alert_offscreen.png";

	EnmRhino *target = nullptr;

protected:
	static void _bind_methods();

public:
	UiHudRhinoAlert() = default;
	~UiHudRhinoAlert() override = default;

	void _ready() override;
	void _process(double delta) override;

	void set_target(EnmRhino *value);

private:
	void setup_layout();
};

} //namespace godot