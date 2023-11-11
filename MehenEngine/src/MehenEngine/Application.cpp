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

	Application::Application()
	{
		MEHEN_ENGINE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);

		// Vertex Array
		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.1f, 0.15f, 0.25f, 1.0f
		};

		m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ E_ShaderDataType::Float3, "a_Position" },
				{ E_ShaderDataType::Float4, "a_Color"}
			};

			m_vertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.m_type), 
				element.m_normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.m_offset);

			//glVertexAttribPointer(index, ShaderDataTypeSize(element.m_type), GL_FLOAT, GL_FALSE, layout.GetStride(), nullptr);
			index++;
		}

		uint32_t indices[3] = { 0, 1, 2 };
		m_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		// Shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;

			void main()
			{
				gl_Position = vec4(a_Position, 1.0);
				v_Color = a_Color;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec4 v_Color;

			void main()
			{
				color = v_Color;
			}
		)";

		m_shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->Bind();
			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_layerStack)
			{
				layer->OnUpdate();
			}

			m_imguiLayer->Begin();

			for (Layer* layer : m_layerStack)
			{
				layer->OnImGuiRender();
			}

			m_imguiLayer->End();

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