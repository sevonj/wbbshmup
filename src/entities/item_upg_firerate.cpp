#include "item_upg_firerate.h"

#include <game.h>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void ItemUpgFirerate::_bind_methods() {}

void ItemUpgFirerate::pickup() {
	Player *player = Game::get_player();
	if (player) {
		player->upgrade_firerate();
	}
	Item::pickup();
}

} //namespace godot