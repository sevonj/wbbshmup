#include "item_upg_firerate.h"

#include <entities/player.h>
#include <game.h>

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