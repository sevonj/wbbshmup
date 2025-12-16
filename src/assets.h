#pragma once

#include <godot_cpp/classes/node3d.hpp>

namespace godot {

/// @brief Asset helper class
class Assets {
public:
	//static constexpr const char * MAT_HITFLASH_PATH = ;

	static Node3D *instance_fallback_model();
};

} //namespace godot