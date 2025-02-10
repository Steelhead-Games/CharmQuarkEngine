#include <D3D12PipelineStateObject.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12PipelineStateObject::D3D12PipelineStateObject(
			const Description& desc,
			RefCountPtr<ID3D12PipelineState> pso
		)
			: PipelineStateObject(desc)
			, m_NativePSO(pso)
		{

		}

		RenderNativeObject D3D12PipelineStateObject::GetNativeObject()
		{
			return RenderNativeObject(m_NativePSO.Get());
		}

		RefCountPtr<ID3D12PipelineState> D3D12PipelineStateObject::GetHandle() const
		{
			return m_NativePSO;
		}
	}
}