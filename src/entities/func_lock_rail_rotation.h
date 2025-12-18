#pragma once

#include <godot_cpp/classes/area3d.hpp>

namespace godot {

/// @brief Area that tells Player to not rotate along rail.
class FuncLockRailRotation : public Area3D {
	GDCLASS(FuncLockRailRotation, Area3D)

protected:
	static void _bind_methods();

public:
	FuncLockRailRotation() = default;
	~FuncLockRailRotation() override = default;

	void _ready() override;

private:
	void on_body_entered(Node3D *body);
	void on_body_exited(Node3D *body);
};

} //namespace godot