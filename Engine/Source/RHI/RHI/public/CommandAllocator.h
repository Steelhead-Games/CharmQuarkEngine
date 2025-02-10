#pragma once

#include <RHICommon.h>

namespace cqe
{
	namespace Render::RHI
	{
		class CommandAllocator : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<CommandAllocator>;

		public:
			virtual void Reset() = 0;
		};
	}
}