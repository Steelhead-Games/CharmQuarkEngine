#pragma once

#include <d3d12.h>

#include <CommandQueue.h>
#include <D3D12Device.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12CommandQueue final : public CommandQueue
		{
		public:
			using Ptr = RefCountPtr<D3D12CommandQueue>;

		public:
			D3D12CommandQueue() = delete;
			D3D12CommandQueue(D3D12Device::Ptr device);
			~D3D12CommandQueue() = default;

		public:
			virtual void ExecuteCommandLists(const std::vector<CommandList::Ptr>& cmdLists) override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12CommandQueue> GetHandle() const;

		private:
			RefCountPtr<ID3D12CommandQueue> m_NativeCommandQueue = nullptr;
		};
	}
}