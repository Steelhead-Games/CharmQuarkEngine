#include <D3D12CommandAllocator.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12CommandAllocator::D3D12CommandAllocator(D3D12Device::Ptr device)
		{
			HRESULT hr = device->GetHandle()->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_NativeCommandAllocator.GetAddressOf()));
			
			assert(SUCCEEDED(hr));
		}

		RenderNativeObject D3D12CommandAllocator::GetNativeObject()
		{
			return RenderNativeObject(m_NativeCommandAllocator.Get());
		}

		RefCountPtr<ID3D12CommandAllocator> D3D12CommandAllocator::GetHandle() const
		{
			return m_NativeCommandAllocator;
		}

		void D3D12CommandAllocator::Reset()
		{
			HRESULT hr = m_NativeCommandAllocator->Reset();
			assert(SUCCEEDED(hr));
		}
	}
}