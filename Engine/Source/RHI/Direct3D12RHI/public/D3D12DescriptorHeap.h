#pragma once

#include <RHI/Direct3D12RHI/export.h>

#include <d3d12.h>
#include <dxgi.h>

#include <Texture.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Device;

		class DIRECT3D_API D3D12DescriptorHeap final : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<D3D12DescriptorHeap>;

		public:
			D3D12DescriptorHeap() = delete;
			D3D12DescriptorHeap(
				RefCountPtr<D3D12Device> device,
				uint32_t NumDescriptors,
				D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType
			);
			~D3D12DescriptorHeap() = default;

		public:
			void Alloc(D3D12_CPU_DESCRIPTOR_HANDLE* cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE* gpuHandle = nullptr);
			void Free(D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE(0));

			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12DescriptorHeap> GetHandle() const;

		private:
			uint32_t m_DescriptorSize = 0;
			uint32_t m_HeapIdx = 0;
			uint32_t m_MaximumDescriptors = 0;
			RefCountPtr<ID3D12DescriptorHeap> m_NativeHeap;
			D3D12_DESCRIPTOR_HEAP_TYPE m_DescriptorHeapType;

			bool m_ShaderVisible = false;

			std::stack<D3D12_CPU_DESCRIPTOR_HANDLE> m_AvailableCPUDescriptors;
			std::stack<D3D12_GPU_DESCRIPTOR_HANDLE> m_AvailableGPUDescriptors;
		};
	}
}