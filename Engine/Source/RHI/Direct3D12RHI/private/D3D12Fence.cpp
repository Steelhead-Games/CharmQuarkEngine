#include <D3D12Fence.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Fence::D3D12Fence(D3D12Device::Ptr device)
		{
			HRESULT hr = device->GetHandle()->CreateFence(0, D3D12_FENCE_FLAG_NONE,
				IID_PPV_ARGS(&m_NativeFence));
			
			assert(SUCCEEDED(hr));
		}

		void D3D12Fence::Sync(CommandQueue::Ptr commandQueue)
		{
			ID3D12CommandQueue* d3d12CommandQueue = (ID3D12CommandQueue*)(commandQueue->GetNativeObject());

			++m_CurrentFence;

			HRESULT hr = d3d12CommandQueue->Signal(m_NativeFence.Get(), m_CurrentFence);
			assert(SUCCEEDED(hr));

			if (m_NativeFence->GetCompletedValue() < m_CurrentFence)
			{
				HANDLE eventHandle = CreateEvent(nullptr, false, false, nullptr);
				assert(eventHandle);

				hr = m_NativeFence->SetEventOnCompletion(m_CurrentFence, eventHandle);
				assert(SUCCEEDED(hr));

				WaitForSingleObject(eventHandle, INFINITE);
				CloseHandle(eventHandle);
			}
		}

		RenderNativeObject D3D12Fence::GetNativeObject()
		{
			return RenderNativeObject(m_NativeFence.Get());
		}

		RefCountPtr<ID3D12Fence> D3D12Fence::GetHandle() const
		{
			return m_NativeFence;
		}
	}
}