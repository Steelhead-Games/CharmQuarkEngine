#include <d3dx12.h>

#include <D3D12Core.h>
#include <D3D12SwapChain.h>
#include <D3D12Texture.h>
#include <D3D12CommandList.h>

#include <RenderCore.h>
#include <Window/IWindow.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12SwapChain::D3D12SwapChain(
			D3D12Device::Ptr device,
			D3D12Factory::Ptr factory,
			D3D12CommandQueue::Ptr commandQueue
		)
			: m_BackBufferFormat(ConvertToDXGIFormat(Settings::BackBuffer))
		{
			m_NativeSwapChain.Reset();

			DXGI_SWAP_CHAIN_DESC sd;
			sd.BufferDesc.Width = Core::g_MainWindowsApplication->GetWidth();
			sd.BufferDesc.Height = Core::g_MainWindowsApplication->GetHeight();
			sd.BufferDesc.RefreshRate.Numerator = 60;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferDesc.Format = m_BackBufferFormat;
			sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd.BufferCount = RenderCore::g_FrameBufferCount;
			sd.OutputWindow = reinterpret_cast<HWND>(Core::g_MainWindowsApplication->GetWindowHandle());
			sd.Windowed = Core::g_MainWindowsApplication->IsWindowed();
			sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			HRESULT hr = factory->GetHandle()->CreateSwapChain(
				commandQueue->GetHandle().Get(),
				&sd,
				m_NativeSwapChain.GetAddressOf());
			assert(SUCCEEDED(hr));

			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
			rtvHeapDesc.NumDescriptors = RenderCore::g_FrameBufferCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			rtvHeapDesc.NodeMask = 0;
			hr = device->GetHandle()->CreateDescriptorHeap(
				&rtvHeapDesc, IID_PPV_ARGS(m_BackBufferHeap.GetAddressOf()));
			assert(SUCCEEDED(hr));
		}

		void D3D12SwapChain::Present()
		{
			HRESULT hr = m_NativeSwapChain->Present(0, 0);
			assert(SUCCEEDED(hr));
			m_CurrentBackBufferIdx = (m_CurrentBackBufferIdx + 1) % RenderCore::g_FrameBufferCount;
		}

		void D3D12SwapChain::MakeBackBufferPresentable(CommandList::Ptr commandList)
		{
			assert(commandList != nullptr);

			D3D12Texture* d3d12BackBuffer = reinterpret_cast<D3D12Texture*>(m_BackBuffer[m_CurrentBackBufferIdx].Get());
			D3D12CommandList* d3d12CommandList = reinterpret_cast<D3D12CommandList*>(commandList.Get());

			if (d3d12BackBuffer->GetCurrentState() != D3D12_RESOURCE_STATE_PRESENT)
			{
				D3D12_RESOURCE_BARRIER ResBarrierTransition = CD3DX12_RESOURCE_BARRIER::Transition(d3d12BackBuffer->GetHandle(),
					d3d12BackBuffer->GetCurrentState(), D3D12_RESOURCE_STATE_PRESENT);
				d3d12CommandList->GetHandle()->ResourceBarrier(1, &ResBarrierTransition);

				d3d12BackBuffer->SetCurrentState(D3D12_RESOURCE_STATE_PRESENT);
			}
		}

		void D3D12SwapChain::Resize(Device::Ptr device, uint32_t width, uint32_t height)
		{
			ID3D12Device* const d3d12Device = (ID3D12Device*)device->GetNativeObject();

			assert(d3d12Device);

			for (int i = 0; i < RenderCore::g_FrameBufferCount; ++i)
			{
				m_BackBuffer[i].Reset();
			}

			HRESULT hr = m_NativeSwapChain->ResizeBuffers(
				RenderCore::g_FrameBufferCount,
				width, height,
				m_BackBufferFormat,
				DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
			assert(SUCCEEDED(hr));

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_BackBufferHeap->GetCPUDescriptorHandleForHeapStart());
			uint32_t RtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			for (UINT i = 0; i < RenderCore::g_FrameBufferCount; i++)
			{
				D3D12Texture::Ptr backBuffer = new D3D12Texture(
					{
						.Dimension = Texture::Dimensions::Two,
						.Width = width,
						.Height = height,
						.MipLevels = 1,
						.Format = ConvertToResourceFormat(m_BackBufferFormat),
						.Flags = Texture::UsageFlags::RenderTarget
					},
					nullptr,
					rtvHeapHandle,
					D3D12_CPU_DESCRIPTOR_HANDLE(0),
					D3D12_CPU_DESCRIPTOR_HANDLE(0));

				hr = m_NativeSwapChain->GetBuffer(i, IID_PPV_ARGS(backBuffer->GetHandleAddress()));
				assert(SUCCEEDED(hr));

				d3d12Device->CreateRenderTargetView(backBuffer->GetHandle().Get(), nullptr, rtvHeapHandle);
				rtvHeapHandle.Offset(1, RtvDescriptorSize);

				m_BackBuffer[i] = backBuffer;
			}
		}

		Texture::Ptr D3D12SwapChain::GetCurrentBackBuffer()
		{
			return m_BackBuffer[m_CurrentBackBufferIdx];
		}

		RenderNativeObject D3D12SwapChain::GetNativeObject()
		{
			return RenderNativeObject(m_NativeSwapChain.Get());
		}

		RefCountPtr<IDXGISwapChain> D3D12SwapChain::GetHandle() const
		{
			return m_NativeSwapChain;
		}
	}
}