#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <Buffer.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Buffer final : public Buffer
		{
		public:
			using Ptr = RefCountPtr<D3D12Buffer>;

		public:
			D3D12Buffer() = delete;
			D3D12Buffer(
				const Description& desc,
				RefCountPtr<ID3D12Resource> resourceDefault = nullptr,
				RefCountPtr<ID3D12Resource> resourceUpload = nullptr
			);
			~D3D12Buffer() = default;

		public:
			virtual void CopyData(int elementIndex, void* data, uint32_t DataSize) override;
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12Resource> GetHandle() const;

		private:
			RefCountPtr<ID3D12Resource> m_NativeResourceDefault = nullptr;
			RefCountPtr<ID3D12Resource> m_NativeResourceUpload = nullptr;
		};
	}
}