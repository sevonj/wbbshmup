#include "ui_debug_wbbstate.h"

namespace godot {

void UiDebugWbbstate::_bind_methods() {
}

UiDebugWbbstate::UiDebugWbbstate() {
	label = nullptr;
}

UiDebugWbbstate::~UiDebugWbbstate() {
}

void UiDebugWbbstate::_ready() {
	wbb_input = WbbInput::get_singleton();

	setup_layout();
}

void UiDebugWbbstate::_process(double delta) {
	if (wbb_input->get_balance_board()) {
		label->set_text("WBB: Connected");
	} else {
		label->set_text("WBB: Not connected. [P] to connect.");
	}
}

void UiDebugWbbstate::setup_layout() {
	label = memnew(Label);
	add_child(label);
}

} //namespace godot