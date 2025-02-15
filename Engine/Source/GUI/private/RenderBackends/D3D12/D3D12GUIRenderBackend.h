#pragma once

#include <GUI/export.h>

#include <RHIContext.h>

struct ImDrawData;

namespace cqe
{
	namespace GUI
	{
		class GUI_API D3D12RenderBackend final
		{
		public:
			D3D12RenderBackend() = delete;
			D3D12RenderBackend(D3D12RenderBackend&) = delete;
			D3D12RenderBackend operator=(D3D12RenderBackend&) = delete;

		public:
			static void Init(Render::RHI::Context::Ptr rhiContext);
			static void Deinit(Render::RHI::Context::Ptr rhiContext);
			static void Deinit();
			static void Render(ImDrawData* drawData);
			static void NewFrame();
		};
	}
}