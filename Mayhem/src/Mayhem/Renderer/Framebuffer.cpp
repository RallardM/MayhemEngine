#include "MayhemPrecompiledHeaders.h"
#include "FrameBuffer.h"

#include "Mayhem/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Mayhem
{
	Ref<Mayhem::Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    MAYHEM_ENGINE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
		}

		MAYHEM_ENGINE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}