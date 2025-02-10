#pragma once

#include <Geometry.h>
#include <RenderCommand.h>
#include <RenderObject.h>
#include <RenderThread.h>

namespace cqe::Render
{
	template void RENDER_ENGINE_API RenderThread::EnqueueCommand<RenderCore::Geometry*, RenderObject*>(ERC command, RenderCore::Geometry* geometry, RenderObject* renderObject);
}