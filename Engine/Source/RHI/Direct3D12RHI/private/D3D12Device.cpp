#include <D3D12Device.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Device::D3D12Device(D3D12Factory::Ptr factory)
		{
			HRESULT hr = D3D12CreateDevice(
				nullptr,
				D3D_FEATURE_LEVEL_11_0,
				IID_PPV_ARGS(&m_NativeDevice));

			if (FAILED(hr))
			{
				RefCountPtr<IDXGIAdapter> warpAdapter;
				hr = factory->GetHandle()->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
				assert(SUCCEEDED(hr));

				hr = D3D12CreateDevice(
					warpAdapter.Get(),
					D3D_FEATURE_LEVEL_11_0,
					IID_PPV_ARGS(&m_NativeDevice));

				assert(SUCCEEDED(hr));
			}
		}

		RenderNativeObject D3D12Device::GetNativeObject()
		{
			return RenderNativeObject(m_NativeDevice.Get());
		}

		RefCountPtr<ID3D12Device> D3D12Device::GetHandle() const
		{
			return m_NativeDevice;
		}
	}
}