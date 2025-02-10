#include <D3D12Factory.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Factory::D3D12Factory()
		{
			HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_NativeFactory));
			assert(SUCCEEDED(hr));
		}

		RenderNativeObject D3D12Factory::GetNativeObject()
		{
			return RenderNativeObject(m_NativeFactory.Get());
		}

		RefCountPtr<IDXGIFactory4> D3D12Factory::GetHandle() const
		{
			return m_NativeFactory;
		}
	}
}