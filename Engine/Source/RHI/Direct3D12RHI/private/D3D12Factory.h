#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

#include <Factory.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Factory final : public Factory
		{
		public:
			using Ptr = RefCountPtr<D3D12Factory>;

		public:
			D3D12Factory();
			~D3D12Factory() = default;

		public:
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<IDXGIFactory4> GetHandle() const;

		private:
			RefCountPtr<IDXGIFactory4> m_NativeFactory = nullptr;
		};
	}
}