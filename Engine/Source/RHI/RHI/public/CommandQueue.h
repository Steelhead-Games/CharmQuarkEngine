#pragma once

#include <RHICommon.h>
#include <CommandList.h>

namespace cqe
{
	namespace Render::RHI
	{
		class CommandQueue : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<CommandQueue>;

		public:
			virtual void ExecuteCommandLists(const std::vector<CommandList::Ptr>& cmdLists) = 0;
		};
	}
}