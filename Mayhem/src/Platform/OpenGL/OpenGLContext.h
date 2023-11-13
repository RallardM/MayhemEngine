#pragma once

#include "Mayhem/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Mayhem
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;

	};
}