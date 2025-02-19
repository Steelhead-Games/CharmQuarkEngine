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
				enum class RootSignatureType
				{
					ConstantBuffer,
					DescriptorTable,
					ShaderResourceView
				};

				uint32_t SlotIndex;
				uint32_t SpaceIndex;
				RootSignatureType RootSignatureType;
				std::string test = "nothing";
			};
			using RootSignature = std::vector<RootSignatureDescription>;

		public:
			using Ptr = RefCountPtr<Technique>;

		public:
			Technique(
				const ShaderInfo& shaderInfo,
				const InputLayout& inputLayout,
				const RootSignature& rootSignature
			)
				: m_ShaderInfo(shaderInfo)
				, m_InputLayout(inputLayout)
				, m_RootSignature(rootSignature)
			{
			}

		protected:
			ShaderInfo m_ShaderInfo;
			InputLayout m_InputLayout;
			RootSignature m_RootSignature;
		};
	}
}