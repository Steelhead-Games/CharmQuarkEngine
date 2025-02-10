#pragma once

#include <d3d12.h>

#include <Fence.h>
#include <D3D12Device.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Fence final : public Fence
		{
		public:
			using Ptr = RefCountPtr<D3D12Fence>;

		public:
			D3D12Fence() = delete;
			D3D12Fence(D3D12Device::Ptr device);
			~D3D12Fence() = default;

		public:
			virtual void Sync(CommandQueue::Ptr commandQueue) override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12Fence> GetHandle() const;

		private:
			uint64_t m_CurrentFence = 0;

			RefCountPtr<ID3D12Fence> m_NativeFence = nullptr;
		};
	}
}