#pragma once

#include <data/souvenirs.h>
#include <entities/item.h>

namespace godot {

/// @brief Souvenirs are unique bonus items, one per stage
class ItemSouvenirVenusArtefact : public Item {
	GDCLASS(ItemSouvenirVenusArtefact, Item)

	static constexpr const char *MDL_PATH = "res://assets/models/mdk_souvenir_venus_artefact.blend";
	static constexpr Souvenir souvenir_id = Souvenir::venus;

protected:
	static void _bind_methods();

public:
	ItemSouvenirVenusArtefact() = default;
	~ItemSouvenirVenusArtefact() override = default;

	String get_display_name() const override { return "Venus artefact for the captain"; }
	String get_pickup_message() const override { return vformat("Picked up souvenir: +%d pts", SOUVENIR_PICKUP_POINTS); }

	void pickup() override;

private:
	void setup_model() override;
};

} //namespace godot