#pragma once

#include <RHICommon.h>

namespace cqe
{
	namespace Render::RHI
	{
		class Factory : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<Factory>;
		};
	}
}