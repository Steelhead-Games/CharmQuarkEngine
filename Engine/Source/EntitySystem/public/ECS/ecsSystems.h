#pragma once

#include <flecs.h>
#include <Geometry.h>

struct GeometryPtr
{
	cqe::RenderCore::Geometry* ptr;
};

namespace cqe
{
	namespace Render
	{
		class RenderThread;
		class RenderObject;
	}

	namespace EntitySystem::ECS
	{
		struct RenderThreadPtr
		{
			Render::RenderThread* ptr;
		};

		struct RenderObjectPtr
		{
			cqe::Render::RenderObject* ptr = nullptr;

			cqe::Render::RenderObject** GetAddressOf()
			{
				return &ptr;
			}
		};

		void RegisterEcsCoreSystems(flecs::world& world);
	}
}
