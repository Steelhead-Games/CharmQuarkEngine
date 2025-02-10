#pragma once

#include <RHICommon.h>
#include <RHICore.h>
#include <Technique.h>

namespace cqe
{
	namespace Render::RHI
	{
		class PipelineStateObject : public RefCounter<RenderBackendResource>
		{
		public:
			struct Description
			{
				Technique::Ptr Technique = nullptr;
				BlendDescription BlendState;
				RasterizerDescription RasterState;
				DepthStencilDescription DepthStencilState;
				PrimitiveTopologyType PrimitiveTopology = PrimitiveTopologyType::Triangle;
				uint32_t NumRenderTargets = 1;
				ResourceFormat RTVFormats[8];
				ResourceFormat DSVFormat;
			};

		public:
			using Ptr = RefCountPtr<PipelineStateObject>;

		public:
			PipelineStateObject(const Description& desc) : m_Description(desc) {}

		protected:
			Description m_Description;
		};
	}
}