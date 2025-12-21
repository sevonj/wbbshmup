#include "item_upg_projectile.h"

#include <game.h>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void ItemUpgProjectile::_bind_methods() {}

void ItemUpgProjectile::pickup() {
	Player *player = Game::get_player();
	if (player) {
		player->upgrade_projectile();
	}
	Item::pickup();
}

} //namespace godot