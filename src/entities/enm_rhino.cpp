#include "enm_rhino.h"

#include <assets.h>
#include <entities/player.h>
#include <entities/projectile_enm_generic.h>
#include <game.h>
#include <ui/ui_hud_rhino_alert.h>
#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/sphere_shape3d.hpp>
#include <godot_cpp/variant/typed_array.hpp>

namespace godot {

void EnmRhino::_bind_methods() {}

EnmRhino::EnmRhino() {
	set_max_health(MAX_HP);
}

EnmRhino::~EnmRhino() {}

void EnmRhino::setup_model() {
	Ref<PackedScene> mdl_res = ResourceLoader::get_singleton()->load(MDL_PATH, "PackedScene");
	if (mdl_res != nullptr) {
		mdl = (Node3D *)mdl_res->instantiate();
	} else {
		mdl = Assets::instance_fallback_model();
	}
	mdl->set_name("mdl");
	add_child(mdl);
}

void EnmRhino::setup_collider() {
	SphereShape3D *sphere = memnew(SphereShape3D);
	sphere->set_radius(COLL_R);
	coll = memnew(CollisionShape3D);
	coll->set_shape(sphere);
	coll->set_name("coll");
	add_child(coll);
}

void EnmRhino::_ready() {
	Enemy::_ready();

	setup_model();
	setup_collider();
}

void EnmRhino::_process(double delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (launched) {
		t_since_launch += delta;
	}
	if (t_since_launch > LIFETIME) {
		queue_free();
	}

	Player *player = Game::get_player();
	if (player && !gave_up) {
		Vector3 origin = get_global_position();
		Vector3 player_pos = player->get_global_position();
		if (origin.distance_to(player_pos) > AI_ALERT_RADIUS) {
			return;
		}
		bool just_launched = !launched;
		if (!launched) {
			launched = true;
			UiHudRhinoAlert *ui = memnew(UiHudRhinoAlert);
			ui->set_target(this);
			Game::get_stage()->add_ui(ui);
			print_line("Rhino alerted");
		}

		Basis basis = get_global_basis();
		Vector3 player_vel = player->get_velocity();

		Vector3 los = player_pos - origin;
		double nav_time = los.length() / SPEED;
		Vector3 player_rel_intercept_pos = los + (player_vel * nav_time);
		Vector3 desired_dir = player_rel_intercept_pos.normalized() * AI_NAVGAIN;
		Basis desired_basis = Basis::looking_at((player_pos + player_rel_intercept_pos + desired_dir) - origin);

		Quaternion rot = Quaternion(basis);
		Quaternion desired_rot = Quaternion(desired_basis);

		if (just_launched) {
			// Snap face player
			set_global_transform(Transform3D(desired_rot, origin));
			return;
		}

		double rot_angle = rot.angle_to(desired_rot);

		// Fixes a bug where transform goes bad once every dozen runs or so.
		// Quaternion::angle_to() likes to return NaN every now and then.
		if (Math::is_nan(rot_angle)) {
			rot_angle = 0.0;
		}

		//print_line("until giveup: ", rot_angle / AI_GIVEUP_ANGLE);
		if (rot_angle >= AI_GIVEUP_ANGLE) {
			gave_up = true;
			print_line("Rhino gave up");
		}

		Quaternion final_rot = rot.slerp(desired_rot, Math::clamp(rot_angle, 0., AI_TURN_RATE * delta));
		set_global_transform(Transform3D(final_rot, origin));
		set_velocity(-get_basis().get_column(2) * SPEED);
	}

	move_and_slide();
}

void EnmRhino::die() {
	emit_signal("died");
	print_line(get_class(), " died");
	coll->set_disabled(true);
	get_tree()->create_timer(HITFLASH_DURATION)->connect("timeout", callable_mp((Node *)this, &EnmRhino::queue_free));
}

String EnmRhino::get_display_name() {
	return "Rhino Missile";
}

String EnmRhino::get_editor_model_path() {
	return "res://assets/characters/enm_boss_powerpark/mdl_powerpark_dummy.obj";
}

} //namespace godot
