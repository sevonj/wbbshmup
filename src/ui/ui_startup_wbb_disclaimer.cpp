#include "ui_startup_wbb_disclaimer.h"

#include <godot_cpp/variant/color.hpp>

namespace godot {

void UiStartupWbbDisclaimer::_bind_methods() {
}

UiStartupWbbDisclaimer::UiStartupWbbDisclaimer() {
	wbb_input = nullptr;

	bgfill = nullptr;
	bgtex = nullptr;
	label = nullptr;
}

UiStartupWbbDisclaimer::~UiStartupWbbDisclaimer() {
}

void UiStartupWbbDisclaimer::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	wbb_input = WbbInput::get_singleton();

	setup_layout();
}

void UiStartupWbbDisclaimer::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
}

void UiStartupWbbDisclaimer::setup_layout() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	set_anchors_and_offsets_preset(PRESET_FULL_RECT);

	bgfill = memnew(ColorRect);
	add_child(bgfill);
	bgtex = memnew(TextureRect);
	add_child(bgtex);

	label = memnew(RichTextLabel);
	add_child(label);
	label->set_text("This game requires a Wii Balance Board");
}

} //namespace godot