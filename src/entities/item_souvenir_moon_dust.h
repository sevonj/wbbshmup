#pragma once

#include <data/souvenirs.h>
#include <entities/item.h>

namespace godot {

/// @brief Souvenirs are unique bonus items, one per stage
class ItemSouvenirMoonDust : public Item {
	GDCLASS(ItemSouvenirMoonDust, Item)

	static constexpr const char *MDL_PATH = "res://assets/models/mdk_souvenir_moon_dust.blend";
	static constexpr Souvenir souvenir_id = Souvenir::moon;

protected:
	static void _bind_methods();

public:
	ItemSouvenirMoonDust() = default;
	~ItemSouvenirMoonDust() override = default;

	String get_display_name() const override { return "Moon dust for the king"; }
	String get_pickup_message() const override { return vformat("Picked up souvenir: +%d pts", SOUVENIR_PICKUP_POINTS); }

	void pickup() override;

private:
	void setup_model() override;
};

} //namespace godot