#include <D3D12Sampler.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Sampler::D3D12Sampler(const Sampler::Description desc, D3D12_CPU_DESCRIPTOR_HANDLE samplerHandle)
			: Sampler(desc),
			m_SamplerHandle(samplerHandle)
		{
		}

		RenderNativeObject D3D12Sampler::GetNativeObject()
		{
			return RenderNativeObject();
		}
	}
}