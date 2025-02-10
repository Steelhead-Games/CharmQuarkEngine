#pragma once

#include <flecs.h>

namespace cqe::Core
{
	class Camera;
}

namespace cqe::EntitySystem::EditorECS
{
	struct CameraPtr
	{
		Core::Camera* ptr;
	};

	struct Position
	{
		float x;
		float y;
		float z;
	};

	struct Speed
	{
		float value;
	};

	void RegisterEditorEcsControlSystems(flecs::world& world);
}
