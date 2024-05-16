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

 // Tutorial : https://youtu.be/BwCqRqqbB1Y?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MayhemPrecompiledHeaders.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace Mayhem
{
	// ########################################################### Vertex Buffer ###########################################################
	Mayhem::OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		MAYHEM_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	Mayhem::OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) 
	{
		MAYHEM_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	Mayhem::OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		MAYHEM_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_rendererId);
	}

	void Mayhem::OpenGLVertexBuffer::Bind() const
	{
		MAYHEM_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	}

	void Mayhem::OpenGLVertexBuffer::Unbind() const
	{
		MAYHEM_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		MAYHEM_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	// ########################################################### Index Buffer ###########################################################

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_count(count)
	{
		MAYHEM_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_rendererId);
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		MAYHEM_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_rendererId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		MAYHEM_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		MAYHEM_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}