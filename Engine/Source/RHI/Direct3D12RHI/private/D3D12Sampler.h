#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <Sampler.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Sampler final : public Sampler
		{
		public:
			using Ptr = RefCountPtr<D3D12Sampler>;

		public:
			D3D12Sampler() = delete;
			D3D12Sampler(
				const Sampler::Description desc,
				D3D12_CPU_DESCRIPTOR_HANDLE samplerHandle
			);
			~D3D12Sampler() = default;

		public:
			virtual RenderNativeObject GetNativeObject() override;

		private:
			D3D12_CPU_DESCRIPTOR_HANDLE m_SamplerHandle;
		};
	}
}


