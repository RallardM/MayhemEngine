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
 // Tutorial : https://youtu.be/YZKEjaCnsjU?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MayhemPrecompiledHeaders.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include "Glad/glad.h"

namespace Mayhem
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		MAYHEM_ENGINE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		MAYHEM_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MAYHEM_ENGINE_ASSERT(status, "Failed to initialize Glad!");

		const GLubyte* version = glGetString(GL_VERSION);
		if (version != nullptr)
		{
			std::string versionStr(reinterpret_cast<const char*>(version));
			MAYHEM_ENGINE_INFO("  Version: {}", versionStr);
		}
		else
		{
			MAYHEM_ENGINE_INFO("Failed to get OpenGL version");
		}

		const GLubyte* vendor = glGetString(GL_VENDOR);
		if (vendor != nullptr) {
			std::string vendorStr(reinterpret_cast<const char*>(vendor));
			MAYHEM_ENGINE_INFO("  Vendor: {}", vendorStr);
		}
		else
		{
			MAYHEM_ENGINE_INFO("Failed to get OpenGL vendor");
		}

		const GLubyte* renderer = glGetString(GL_RENDERER);
		if (renderer != nullptr)
		{
			std::string rendererStr(reinterpret_cast<const char*>(renderer));
			MAYHEM_ENGINE_INFO("  Renderer: {}", rendererStr);
		}
		else
		{
			MAYHEM_ENGINE_INFO("Failed to get OpenGL renderer");
		}

		#ifdef MAYHEM_ENABLE_ASSERTS
				int versionMajor;
				int versionMinor;
				glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
				glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

				MAYHEM_ENGINE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Mayhem requires at least OpenGL version 4.5!");
		#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		MAYHEM_PROFILE_FUNCTION();

		glfwSwapBuffers(m_windowHandle);
	}
}