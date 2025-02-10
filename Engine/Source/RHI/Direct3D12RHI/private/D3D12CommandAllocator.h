#pragma once

#include <d3d12.h>

#include <CommandAllocator.h>
#include <D3D12Device.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12CommandAllocator final : public CommandAllocator
		{
		public:
			using Ptr = RefCountPtr<D3D12CommandAllocator>;

		public:
			D3D12CommandAllocator() = delete;
			D3D12CommandAllocator(D3D12Device::Ptr device);
			~D3D12CommandAllocator() = default;

		public:
			virtual void Reset() override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12CommandAllocator> GetHandle() const;

		private:
			RefCountPtr<ID3D12CommandAllocator> m_NativeCommandAllocator = nullptr;
		};
	}
}