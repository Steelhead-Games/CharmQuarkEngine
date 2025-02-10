#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <D3D12CommandQueue.h>
#include <D3D12Device.h>
#include <D3D12Factory.h>
#include <SwapChain.h>

#include <RenderCore.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12SwapChain final : public SwapChain
		{
		public:
			using Ptr = RefCountPtr<D3D12SwapChain>;

		public:
			D3D12SwapChain() = delete;
			D3D12SwapChain(
				D3D12Device::Ptr device,
				D3D12Factory::Ptr factory,
				D3D12CommandQueue::Ptr commandQueue
			);
			~D3D12SwapChain() = default;

		public:
			virtual void Present() override;
			virtual void MakeBackBufferPresentable(CommandList::Ptr commandList) override;

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<IDXGISwapChain> GetHandle() const;

		public:
			virtual void Resize(Device::Ptr device, uint32_t width, uint32_t height) override;
			virtual Texture::Ptr GetCurrentBackBuffer() override;

		private:
			Texture::Ptr m_BackBuffer[RenderCore::g_FrameBufferCount];
			RefCountPtr<IDXGISwapChain> m_NativeSwapChain = nullptr;
			RefCountPtr<ID3D12DescriptorHeap> m_BackBufferHeap = nullptr;
			DXGI_FORMAT m_BackBufferFormat;
		};
	}
}