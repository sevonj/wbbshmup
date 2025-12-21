#pragma once

#include <data/damage_info.h>
#include <entities/item.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

namespace godot {

/// @brief Upgrades player projectile count by one.
class ItemUpgProjectile : public Item {
	GDCLASS(ItemUpgProjectile, Item)

protected:
	static void _bind_methods();

public:
	ItemUpgProjectile() = default;
	~ItemUpgProjectile() override = default;

	void pickup() override;
};

} //namespace godot