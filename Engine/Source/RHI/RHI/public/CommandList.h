#pragma once

#include <Colors.h>
#include <RHICommon.h>
#include <CommandAllocator.h>
#include <PipelineStateObject.h>
#include <Texture.h>
#include <Mesh.h>

namespace cqe
{
	namespace Render::RHI
	{
		class CommandList : public RefCounter<RenderBackendResource>
		{
		public:
			using Ptr = RefCountPtr<CommandList>;

		public:
			virtual void ClearRenderTarget(Texture::Ptr renderTarget, RenderCore::Color& color) = 0;
			virtual void ClearDepthStencilView(Texture::Ptr depthStencil, ClearFlags::Flag clearFlag, float depth, uint8_t stencil) = 0;
			virtual void SetRenderTargets(uint32_t TargetsNum, Texture::Ptr renderTarget, Texture::Ptr depthStencil) = 0;
			virtual void SetViewport(const Viewport& viewport) = 0;
			virtual void SetScissorRect(const Rect& scissorRect) = 0;
			virtual void Close() = 0;
			virtual void Reset() = 0;

			virtual void SetPipelineStateObject(PipelineStateObject::Ptr pso) = 0;
			virtual void SetMesh(Mesh::Ptr mesh) = 0;
			virtual void SetTextureDescriptorTable(uint32_t parameterIdx, Texture::Ptr texture) = 0;
			virtual void SetGraphicsConstantBuffer(uint32_t ParameterIdx, Buffer::Ptr buffer, uint32_t bufferOffset = 0) = 0;
			virtual void SetGraphicsDescriptorTable(uint32_t parameterIdx, uint64_t ptr) = 0;
			virtual void SetGraphicsShaderResourseView(uint32_t parameterIdx, Buffer::Ptr buffer, uint32_t bufferOffset) = 0;
			virtual void SetTechnique(Technique::Ptr technique) = 0;
			virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
			
			virtual void DrawIndexedInstanced(
				uint32_t IndexCountPerInstance,
				uint32_t InstanceCount,
				uint32_t StartIndexLocation,
				int32_t BaseVertexLocation,
				uint32_t StartInstanceLocation
			) = 0;

		public:
			virtual CommandAllocator::Ptr GetAllocator() const = 0;
		};
	}
}