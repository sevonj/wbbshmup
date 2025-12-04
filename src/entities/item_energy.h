#pragma once

#include <data/damage_info.h>
#include <entities/item.h>
#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/collision_shape3d.hpp>

namespace godot {

/// @brief Player pickup-able ItemEnergy. Picked up when touching player.
class ItemEnergy : public Item {
	GDCLASS(ItemEnergy, Item)

private:
protected:
	static void _bind_methods();

public:
	ItemEnergy();
	~ItemEnergy();

	void _ready() override;
	void _process(double delta) override;
	// void _physics_process(double delta) override;
	void pickup() override;
};

} //namespace godot