#pragma once

#include <data/damage_info.h>
#include <entities/item.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

namespace godot {

/// @brief Upgrades player projectile count by one.
class ItemUpgFirerate : public Item {
	GDCLASS(ItemUpgFirerate, Item)

protected:
	static void _bind_methods();

public:
	ItemUpgFirerate() = default;
	~ItemUpgFirerate() override = default;

	void pickup() override;
};

} //namespace godot