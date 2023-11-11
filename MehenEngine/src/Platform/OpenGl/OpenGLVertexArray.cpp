/*
 * Copyright 2023 TheCherno/Hazel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 // Tutorial : https://youtu.be/rkxrw8dNrvI?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MhnPCHeader.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace MehenEngine
{
	// Temporary
	static GLenum ShaderDataTypeToOpenGLBaseType(E_ShaderDataType type)
	{
		switch (type)
		{
		case E_ShaderDataType::Float:    return GL_FLOAT;
		case E_ShaderDataType::Float2:   return GL_FLOAT;
		case E_ShaderDataType::Float3:   return GL_FLOAT;
		case E_ShaderDataType::Float4:   return GL_FLOAT;
		case E_ShaderDataType::Mat3:     return GL_FLOAT;
		case E_ShaderDataType::Mat4:     return GL_FLOAT;
		case E_ShaderDataType::Int:      return GL_INT;
		case E_ShaderDataType::Int2:     return GL_INT;
		case E_ShaderDataType::Int3:     return GL_INT;
		case E_ShaderDataType::Int4:     return GL_INT;
		case E_ShaderDataType::Bool:     return GL_BOOL;
		}

		MEHEN_ENGINE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	MehenEngine::OpenGLVertexArray::OpenGLVertexArray()
	{
		glGenVertexArrays(1, &m_rendererId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_rendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		MEHEN_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_rendererId);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_vertexBufferIndex);
			glVertexAttribPointer(m_vertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.m_type),
				element.m_normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.m_offset);
			m_vertexBufferIndex++;
		}

		m_vertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_rendererId);
		indexBuffer->Bind();

		m_indexBuffer = indexBuffer;
	}
}