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

#include "MayhemPrecompiledHeaders.h"
#include "Application.h"

#include "Mayhem/Core/Log.h"
#include "Input.h"

#include "Mayhem/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Mayhem
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::_Instance = nullptr;

	Application::Application()
	{
		MAYHEM_PROFILE_FUNCTION();

		MAYHEM_ENGINE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;
		m_window = Window::Create();
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		//m_window->SetVSync(false);

		Renderer::Init();

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);
	}

	Application::~Application()
	{
		MAYHEM_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::Run()
	{
		MAYHEM_PROFILE_FUNCTION();

		while (m_running)
		{
			MAYHEM_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized)
			{
				{
					MAYHEM_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_layerStack)
					{
						layer->OnUpdate(timestep);
					}
				}

				m_imguiLayer->Begin();

				{
					MAYHEM_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : m_layerStack)
					{
						layer->OnImGuiRender();
					}
				}

				m_imguiLayer->End();
			}


			m_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		MAYHEM_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		MAYHEM_PROFILE_FUNCTION();

		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		MAYHEM_PROFILE_FUNCTION();

		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		MAYHEM_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}

		m_minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}