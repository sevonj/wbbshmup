#pragma once

#include <singleton/wbb_input.h>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

namespace godot {

class UiStartupWbbDisclaimer : public MarginContainer {
	GDCLASS(UiStartupWbbDisclaimer, MarginContainer)

private:
	WbbInput *wbb_input;

	ColorRect *bgfill;
	TextureRect *bgtex;
	RichTextLabel *label;

	void setup_layout();

protected:
	static void _bind_methods();

public:
	UiStartupWbbDisclaimer();
	~UiStartupWbbDisclaimer();

	void _ready() override;
	void _process(double delta) override;
};

} //namespace godot