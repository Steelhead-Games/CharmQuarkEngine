#pragma once

#include <RHIContext.h>
#include <RHI/RHI/export.h>

namespace cqe
{
	namespace Render::RHI
	{
		class RHI_API Helper final
		{
		public:
			// RHI - e.g. "Vulkan" or "D3D12"
			static Context::Ptr CreateRHI(const std::string& RHIName);
			static Type GetRHIType();

		private:
			static Type s_rhiType;
		};
	}
}