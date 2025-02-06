#pragma once

#include <RHIContext.h>
#include <RHI/RHI/export.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		class RHI_API RHIHelper final
		{
		public:
			// RHI - e.g. "Vulkan" or "D3D12"
			static RHIContext::Ptr CreateRHI(const std::string& RHIName);
			static RHIType GetRHIType();

		private:
			static RHIType s_rhiType;
		};
	}
}