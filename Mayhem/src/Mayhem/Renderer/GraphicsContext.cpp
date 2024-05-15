#include "MayhemPrecompiledHeaders.h"
#include "GraphicsContext.h"

#include "Mayhem/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Mayhem
{
	Scope<GraphicsContext> Mayhem::GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    MAYHEM_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		MAYHEM_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}