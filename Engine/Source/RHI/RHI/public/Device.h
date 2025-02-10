#pragma once

#include <RHICommon.h>

namespace cqe
{
	namespace Render::RHI
	{
		class Device : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<Device>;			
		};
	}
}