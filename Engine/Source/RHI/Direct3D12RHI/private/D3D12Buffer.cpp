#include <D3D12Buffer.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Buffer::D3D12Buffer(
			const Description& desc,
			RefCountPtr<ID3D12Resource> resourceDefault,
			RefCountPtr<ID3D12Resource> resourceUpload
		)
			: Buffer(desc)
		{
			m_NativeResourceDefault = resourceDefault;
			m_NativeResourceUpload = resourceUpload;
		}

		void D3D12Buffer::CopyData(int elementIndex, void* data, uint32_t DataSize)
		{
			assert(m_NativeResourceUpload != nullptr);
			assert(m_Description.UsageFlag != Buffer::UsageFlag::GpuReadOnly);

			uint8_t* mappedData;
			HRESULT hr = m_NativeResourceUpload->Map(0, nullptr, reinterpret_cast<void**>(&mappedData));
			assert(SUCCEEDED(hr));

			memcpy(&mappedData[elementIndex * m_Description.ElementSize], data, DataSize);

			m_NativeResourceUpload->Unmap(0, nullptr);

			mappedData = nullptr;
		}

		RenderNativeObject D3D12Buffer::GetNativeObject()
		{
			if (m_NativeResourceDefault != nullptr)
			{
				return RenderNativeObject(m_NativeResourceDefault.Get());
			}

			return RenderNativeObject(m_NativeResourceUpload.Get());
		}

		RefCountPtr<ID3D12Resource> D3D12Buffer::GetHandle() const
		{
			if (m_NativeResourceDefault != nullptr)
			{
				return m_NativeResourceDefault;
			}

			return m_NativeResourceUpload;
		}
	}
}