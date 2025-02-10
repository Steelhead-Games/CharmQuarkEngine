#pragma once

#include <RHICommon.h>
#include <CommandQueue.h>

namespace cqe
{
	namespace Render::RHI
	{
		class Fence : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<Fence>;

		public:
			virtual void Sync(CommandQueue::Ptr commandQueue) = 0;
		};
	}
}