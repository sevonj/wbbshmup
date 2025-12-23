#pragma once

#include <entities/item.h>

namespace godot {

/// @brief Upgrades player projectile count by one.
class ItemUpgProjectile : public Item {
	GDCLASS(ItemUpgProjectile, Item)

protected:
	static void _bind_methods();

public:
	ItemUpgProjectile() = default;
	~ItemUpgProjectile() override = default;

	String get_display_name() const override { return "Projectile upgrade"; }

	void pickup() override;
};

} //namespace godot