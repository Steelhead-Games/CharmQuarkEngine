#pragma once

#include <d3d12.h>
#include <dxgi.h>

#include <PipelineStateObject.h>

namespace cqe
{
	namespace Render::RHI
	{
		class D3D12PipelineStateObject final : public PipelineStateObject
		{
		public:
			using Ptr = RefCountPtr<D3D12PipelineStateObject>;

		public:
			D3D12PipelineStateObject() = delete;
			D3D12PipelineStateObject(const Description& desc, RefCountPtr<ID3D12PipelineState> pso);
			~D3D12PipelineStateObject() = default;

		public:
			virtual RenderNativeObject GetNativeObject() override;
			RefCountPtr<ID3D12PipelineState> GetHandle() const;

		private:
			RefCountPtr<ID3D12PipelineState> m_NativePSO = nullptr;
		};
	}
}