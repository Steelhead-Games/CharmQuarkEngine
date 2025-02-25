#pragma once

#include <RHICommon.h>
#include <RHICore.h>

namespace cqe
{
	namespace Render::RHI
	{
		// It represents render backend resource so we still inherit it from RenderBackendResource
		class Technique : public RefCounter<RenderBackendResource>
		{
		public:
			struct ShaderInfoDescription
			{
				enum class ShaderType
				{
					VertexShader = 0,
					PixelShader,

					Undefined
				};

				ShaderType Type;
				std::string ShaderFile;
				std::string EntryPoint;
			};
			using ShaderInfo = std::vector<ShaderInfoDescription>;

			struct InputLayoutDescription
			{
				enum class Classification
				{
					PerVertex = 0,
					PerInstance
				};

				std::string SemanticName;
				uint32_t Index;
				ResourceFormat Format;
				uint32_t InputSlot;
				Classification InputSlotClass;
				uint32_t InstanceDataStepRate;
			};
			using InputLayout = std::vector<InputLayoutDescription>;

			struct RootSignatureDescription
			{
				std::string Type;
			};
			using RootSignature = std::vector<RootSignatureDescription>;

		public:
			using Ptr = RefCountPtr<Technique>;

		public:
			Technique(
				const ShaderInfo& shaderInfo,
				const InputLayout& inputLayout,
				const RootSignatureDescription& rootSignatureDescription
			)
				: m_ShaderInfo(shaderInfo)
				, m_InputLayout(inputLayout)
				, m_RootSignatureDescription(rootSignatureDescription)
			{
			}

		protected:
			ShaderInfo m_ShaderInfo;
			InputLayout m_InputLayout;
			RootSignatureDescription m_RootSignatureDescription;
		};
	}
}