#pragma once

#include <wbb_input.h>
#include <godot_cpp/classes/color_rect.hpp>
#include <godot_cpp/classes/margin_container.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/texture_rect.hpp>

namespace godot {

class UiStartupWbbDisclaimer : public MarginContainer {
	GDCLASS(UiStartupWbbDisclaimer, MarginContainer)

	WbbInput *wbb_input = nullptr;

	ColorRect *bgfill = nullptr;
	TextureRect *bgtex = nullptr;
	RichTextLabel *label = nullptr;

protected:
	static void _bind_methods();

public:
	UiStartupWbbDisclaimer() = default;
	~UiStartupWbbDisclaimer() override = default;

	void _ready() override;
	void _process(double delta) override;

private:
	void setup_layout();
};

} //namespace godot