#include "item_upg_projectile.h"

#include <entities/player.h>
#include <game.h>

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