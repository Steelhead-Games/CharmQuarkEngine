#include <D3D12Technique.h>

namespace cqe
{
	namespace Render::RHI
	{
		D3D12Technique::D3D12Technique(
			const ShaderInfo& shaderInfo,
			const InputLayout& inputLayout,
			const RootSignature& rootSignature,
			RefCountPtr<ID3D12RootSignature> d3d12RootSiungature,
			ShaderBlobList&& shaderBlobList,
			InputLayoutList&& inputLayoutList
		) 
			: Technique(shaderInfo, inputLayout, rootSignature)
			, m_d3d12RootSignature(d3d12RootSiungature)
			, m_ShaderBlobList(std::move(shaderBlobList))
			, m_InputLayoutList(std::move(inputLayoutList))
		{
		}


		RenderNativeObject D3D12Technique::GetNativeObject()
		{
			assert(0 && "D3D12Technique can't have the native object");
			return nullptr;
		}
	};
}