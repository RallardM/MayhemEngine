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

#include "MhnPCHeader.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace MehenEngine
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case E_RendererAPI::None:   MEHEN_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case E_RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
		}

		MEHEN_ENGINE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case E_RendererAPI::None:   MEHEN_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case E_RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, count);
		}

		MEHEN_ENGINE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}