#pragma once

#include <Buffer.h>

namespace cqe::Render
{
	namespace RHI
	{
		class Mesh
		{
		public:
			using ID = uint64_t;
			using Ptr = Mesh*;

			struct VertexBufferDescription
			{
				uint32_t Count = 1;
				uint32_t ElementSize = 1;
				void* initData = nullptr;
			};

			struct IndexBufferDescription
			{
				uint32_t Count = 1;
				ResourceFormat Format = ResourceFormat::R16_UINT;
				void* initData = nullptr;
			};

		public:
			Mesh() = delete;
			Mesh(Buffer::Ptr vertexBuffer, Buffer::Ptr indexBuffer, ResourceFormat IndexFormat)
				: m_VertexBuffer(vertexBuffer)
				, m_IndexBuffer(indexBuffer)
				, m_IndexFormat(IndexFormat)
			{}

			const Buffer::Ptr& GetVertexBuffer() const { return m_VertexBuffer; }
			const Buffer::Ptr& GetIndexBuffer() const { return m_IndexBuffer; }
			const ResourceFormat& GetIndexFormat() const { return m_IndexFormat; }

		protected:
			Buffer::Ptr m_VertexBuffer;
			Buffer::Ptr m_IndexBuffer;
			ResourceFormat m_IndexFormat;
		};
	}
}