#include "enm_boss_clobster.h"

#include <assets.h>
#include <game.h>
#include <godot_cpp/classes/capsule_shape3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

namespace godot {

void EnmBossClobster::_bind_methods() {}

void EnmBossClobster::_ready() {
	Enemy::_ready();

	setup_model();
	setup_collider();

	create_bossbar();
}

void EnmBossClobster::_process(double delta) {}

void EnmBossClobster::create_bossbar() {
	//UiBossHealthBar *bossbar = memnew(UiBossHealthBar);
	//bossbar->set_target(this);
	//Game::get_stage()->add_ui(bossbar);
}

void EnmBossClobster::setup_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res.is_valid()) {
		mdl = cast_to<Node3D>(mdl_res->instantiate());
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

void EnmBossClobster::setup_collider() {
	CapsuleShape3D *capsule = memnew(CapsuleShape3D);
	capsule->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(capsule);
	coll->set_name("coll");
	add_child(coll);
	coll->set_position(Vector3(0, COLL_H / 2.0, 0));
}

} //namespace godot