#pragma once

#include <RHICommon.h>
#include <CommandList.h>
#include <Device.h>
#include <Texture.h>

namespace cqe
{
	namespace Render::RHI
	{
		class SwapChain : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<SwapChain>;

		public:
			virtual void Present() = 0;
			virtual void MakeBackBufferPresentable(CommandList::Ptr commandList) = 0;

		public:
			virtual void Resize(Device::Ptr device, uint32_t width, uint32_t height) = 0;
			virtual Texture::Ptr GetCurrentBackBuffer() = 0;

			uint64_t GetCurrentBackBufferIdx() const { return m_CurrentBackBufferIdx; }

		protected:
			uint64_t m_CurrentBackBufferIdx = 0;
		};
	}
}