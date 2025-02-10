#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <Technique.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12Technique final : public Technique
		{
		public:
			using Ptr = RefCountPtr<D3D12Technique>;
			using ShaderBlobList = std::unordered_map<Technique::ShaderInfoDescription::ShaderType, RefCountPtr<ID3DBlob>>;
			using InputLayoutList = std::vector<D3D12_INPUT_ELEMENT_DESC>;

		public:
			D3D12Technique() = delete;
			D3D12Technique(
				const ShaderInfo& shaderInfo,
				const InputLayout& inputLayout,
				const RootSignature& rootSignature,
				RefCountPtr<ID3D12RootSignature> d3d12RootSiungature,
				ShaderBlobList&& shaderBlobList,
				InputLayoutList&& inputLayoutList
			);
			~D3D12Technique() = default;

		public:
			virtual RenderNativeObject GetNativeObject() override;

			const InputLayoutList& GetInputLayout() const { return m_InputLayoutList; }
			const ShaderBlobList& GetShaderBlobs() const { return m_ShaderBlobList; }
			RefCountPtr<ID3D12RootSignature> GetRootSignature() { return m_d3d12RootSignature; }
		private:
			RefCountPtr<ID3D12RootSignature> m_d3d12RootSignature;
			ShaderBlobList m_ShaderBlobList;
			InputLayoutList m_InputLayoutList;
		};
	}
}