#include "item_souvenir_venus_artefact.h"

#include <assets.h>
#include <game.h>
#include <stage.h>
#include <godot_cpp/classes/resource_loader.hpp>

namespace godot {

void ItemSouvenirVenusArtefact::_bind_methods() {}

void ItemSouvenirVenusArtefact::pickup() {
	Stage *stage = Game::get_stage();
	if (stage) {
		stage->add_score(SOUVENIR_PICKUP_POINTS);
	}
	Item::pickup();
}

void ItemSouvenirVenusArtefact::setup_model() {
	Node *mdl_node = get_node_or_null("mdl");
	mdl = cast_to<Node3D>(mdl_node);
	if (mdl) {
		return;
	} else if (mdl_node) {
		mdl_node->set_name("mdl_wtf");
	}

	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res.is_valid()) {
		mdl = cast_to<Node3D>(mdl_res->instantiate());
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

} //namespace godot