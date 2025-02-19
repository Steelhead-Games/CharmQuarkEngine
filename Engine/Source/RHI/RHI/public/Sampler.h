#pragma once

#include <RHICommon.h>
#include <RHICore.h>
#include <Colors.h>

#ifdef max
#undef max
#endif

namespace cqe
{
	namespace Render::RHI
	{
		class Sampler : public RefCounter<RenderBackendResource>
		{
		public:
			struct Description
			{
				SamplerMipFilter filter = SamplerMipFilter::MIN_MAG_LINEAR;
				SamplerAddressMode addressModeU = SamplerAddressMode::BORDER;
				SamplerAddressMode addressModeV = SamplerAddressMode::BORDER;
				SamplerAddressMode addressModeW = SamplerAddressMode::BORDER;
				float mipLodBias = 0;
				uint32_t maxAnisotropy = 1;
				ComparisonFunc comparisonFunction = ComparisonFunc::Always;
				RenderCore::Color borderColor = RenderCore::Color( 0.0f, 0.0f, 0.0f, 1.0f );
				float minLod = 0;
				float maxLod = std::numeric_limits<float>::max();
			};

		public:
			using Ptr = RefCountPtr<Sampler>;

		public:
			Sampler() = delete;
			Sampler(const Description& desc)
				: m_Description(desc)
			{
			}

		public:
			inline const Description& GetDesc() const { return m_Description; }

		protected:
			Description m_Description;
		};
	}
}