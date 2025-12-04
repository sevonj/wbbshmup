#include "register_types.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <entities/camera_rig.h>
#include <entities/camera_rig_orbit.h>
#include <entities/camera_rig_tp.h>
#include <entities/character.h>
#include <entities/enemy.h>
#include <entities/enm_boss_cocklobster.h>
#include <entities/item.h>
#include <entities/item_energy.h>
#include <entities/player.h>
#include <entities/projectile.h>
#include <entities/projectile_player_laser.h>
#include <singleton/debug_draw.h>
#include <singleton/wbb_input.h>
#include <stage.h>

using namespace godot;

void initialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
	GDREGISTER_CLASS(CameraRig);
	GDREGISTER_CLASS(CameraRigOrbit);
	GDREGISTER_CLASS(CameraRigTp);
	GDREGISTER_CLASS(Character);
	GDREGISTER_CLASS(Enemy);
	GDREGISTER_CLASS(EnmBossCocklobster);
	GDREGISTER_CLASS(Item);
	GDREGISTER_CLASS(ItemEnergy);
	GDREGISTER_CLASS(Player);
	GDREGISTER_CLASS(Projectile);
	GDREGISTER_CLASS(ProjectilePlayerLaser);

	GDREGISTER_CLASS(DebugDraw);
	GDREGISTER_CLASS(WbbInput);

	GDREGISTER_CLASS(Stage);
}

void uninitialize_gdextension_types(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization
GDExtensionBool GDE_EXPORT game_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
	init_obj.register_initializer(initialize_gdextension_types);
	init_obj.register_terminator(uninitialize_gdextension_types);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}