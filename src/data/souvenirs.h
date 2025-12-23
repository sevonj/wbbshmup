#pragma once

#include <godot_cpp/variant/variant.hpp>

namespace godot {

static constexpr int32_t SOUVENIR_PICKUP_POINTS = 5000;

enum Souvenir {
	mercury = 1,
	venus = 1 << 1,
	earth = 1 << 2,
	moon = 1 << 3,
	mars = 1 << 4,
	ceres = 1 << 5,
	jupiter = 1 << 6,
	saturn = 1 << 7,
	uranus = 1 << 8,
	neptune = 1 << 9,
	pluto = 1 << 10,
};

} //namespace godot
