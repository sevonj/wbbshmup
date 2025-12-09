#include "assets.h"

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace godot {

/// @brief Instances a 3D model that says "ERROR". Use this when loading a model failed.
/// @return Instance of res://assets/models/mdl_debug_error.glb
Node3D *Assets::instance_error_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load("res://assets/models/mdl_debug_error.glb", "PackedScene");
	return (Node3D *)mdl_res->instantiate();
}

} //namespace godot