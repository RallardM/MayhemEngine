#pragma once

#include "MehenEngine/Renderer/VertexArray.h"

namespace MehenEngine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		//virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; }
		//virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		uint32_t m_rendererId;
		uint32_t m_vertexBufferIndex = 0;
	};
}