#include "item.h"

#include <assets.h>
#include <config.h>
#include <consts.h>
#include <debug_draw.h>
#include <game.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>

namespace godot {

void Item::_bind_methods() {
	ClassDB::bind_method(D_METHOD("pickup"), &Item::pickup);
}

void Item::_ready() {
	set_collision_layer(COL_LAYER_NONE);
	set_collision_mask(COL_MASK_ITEMS);

	setup_collider();
	setup_model();
}

void Item::_process(double delta) {
	if (mdl) {
		mdl->rotate_y(ROT_SPEED * delta);

		// Enter the world with a funny bounce
		float spawn_anim_t = Math::clamp(lifetime, 0.0, 1.0);
		const float h_scale = 4.0;
		float h = Math::abs(sin(spawn_anim_t * M_PI * 2.0)) * (1.0 - spawn_anim_t) * h_scale;
		mdl->set_position(Vector3(0., h, 0.));
		mdl->set_scale(Vector3(1., 1., 1.) * spawn_anim_t);
	}

	lifetime += delta;

	if (Engine::get_singleton()->is_editor_hint()) {
		if (lifetime >= 3.0) {
			lifetime -= 3.0;
		}
		return;
	}

	if (config::debug_draw_colliders) {
		DebugDraw::draw_sphere3d(get_global_position(), coll_sphere->get_radius(), COLOR_DEBUG_COLL);
	}
}

void Item::_physics_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	Ref<KinematicCollision3D> collision = move_and_collide(Vector3());
	if (collision.is_valid()) {
		Player *playa = cast_to<Player>(collision->get_collider());
		if (playa) {
			pickup();
		}
	}
}

void Item::setup_collider() {
	coll_sphere = (Ref<SphereShape3D>)memnew(SphereShape3D);
	coll_sphere->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(coll_sphere);
	coll->set_name("coll");
	add_child(coll);
}

void Item::setup_model() {
	Node *mdl_node = get_node_or_null("mdl");
	mdl = cast_to<Node3D>(mdl_node);
	if (mdl) {
		return;
	} else if (mdl_node) {
		mdl_node->set_name("mdl_wtf");
	}

	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(DEFAULT_MDL_PATH, "PackedScene");
	if (mdl_res.is_valid()) {
		mdl = cast_to<Node3D>(mdl_res->instantiate());
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

void Item::pickup() {
	print_line("Picked up an item!");
	queue_free();
}

} //namespace godot