#pragma once

#include <d3d12.h>

#include <Device.h>
#include <D3D12Factory.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Device final : public Device
		{
		public:
			using Ptr = RefCountPtr<D3D12Device>;

		public:
			D3D12Device() = delete;
			D3D12Device(D3D12Factory::Ptr factory);
			~D3D12Device() = default;

		public:
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12Device> GetHandle() const;
			bool CheckCompatibility() const;

		private:
			RefCountPtr<ID3D12Device> m_NativeDevice = nullptr;
		};
	}
}