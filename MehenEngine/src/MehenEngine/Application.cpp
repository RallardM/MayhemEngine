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

#include "MhnPCHeader.h"
#include "Application.h"

#include "MehenEngine/Log.h"
#include "Input.h"

#include <Glad/glad.h>

namespace MehenEngine
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::_Instance = nullptr;

	Application::Application()
	{
		MHN_ENGINE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_layerStack)
			{
				layer->OnUpdate();
			}

			auto[x, y] = Input::GetMousePosition();
			MEHEN_ENGINE_TRACE("{0}, {1}", x, y);
				
			m_window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

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
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
}