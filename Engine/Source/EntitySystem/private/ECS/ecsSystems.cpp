#pragma once

#include <ECS/ecsSystems.h>
#include <flecs.h>
#include <Geometry.h>
#include <RenderThread.h>
#include <RenderObject.h>
#include <RenderEngine.h>

namespace cqe::EntitySystem::ECS
{
	void RegisterEcsCoreSystems(flecs::world& world)
	{
		static const RenderThreadPtr* renderThread = world.get<RenderThreadPtr>();

		world.system<const GeometryPtr>()
			.each([&](flecs::entity e, const GeometryPtr& geometry)
		{
			Render::RenderObject* renderObjectPtr = new Render::RenderObject(nullptr);
			e.set(RenderObjectPtr{ renderObjectPtr });
			renderThread->ptr->EnqueueCommand(
				[geometry, renderObjectPtr]() { renderThread->ptr->GetRenderEngine()->CreateRenderObject(geometry.ptr, renderObjectPtr); }
			);
			e.remove<GeometryPtr>();
		});
	}
}
