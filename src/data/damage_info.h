#pragma once

#include <godot_cpp/variant/variant.hpp>

namespace godot {

struct DamageInfo {
	/// @brief Amount of damage
	int32_t value;
	/// @brief Knockback impulse
	Vector3 impulse;

	DamageInfo(int32_t v) :
			value(v),
			impulse(Vector3()) {}

	DamageInfo(int32_t v, Vector3 i) :
			value(v),
			impulse(i) {}
};

} //namespace godot
