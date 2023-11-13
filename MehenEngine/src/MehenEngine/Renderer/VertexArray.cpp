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

#include "MehenPrecompiledHeaders.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace MehenEngine
{
    VertexArray* MehenEngine::VertexArray::Create()
    {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:   MEHEN_ENGINE_ASSERT(false, "RendererAPI::API::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
		}

		MEHEN_ENGINE_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
    }
}