#pragma once

#include <entities/item.h>

namespace godot {

/// @brief Upgrades player projectile count by one.
class ItemUpgFirerate : public Item {
	GDCLASS(ItemUpgFirerate, Item)

protected:
	static void _bind_methods();

public:
	ItemUpgFirerate() = default;
	~ItemUpgFirerate() override = default;

	String get_display_name() const override { return "Firerate upgrade"; }

	void pickup() override;
};

} //namespace godot