#pragma once

#include <flecs.h>

namespace cqe::Core
{
	class Camera;
	class Controller;
}

struct ControllerPtr
{
	cqe::Core::Controller* ptr;
};

struct JumpSpeed
{
	float value;
};

struct CameraPtr
{
	cqe::Core::Camera* ptr;
};

void RegisterEcsControlSystems(flecs::world& world);

