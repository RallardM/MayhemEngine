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

#include "MehenEngine/Renderer/Renderer.h"

namespace MehenEngine
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::_Instance = nullptr;

	Application::Application()
		: m_camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		MEHEN_ENGINE_ASSERT(!_Instance, "Application already exists!");
		_Instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_imguiLayer = new ImGuiLayer();
		PushOverlay(m_imguiLayer);

		m_vertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.1f, 0.15f, 0.25f, 1.0f
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ E_ShaderDataType::Float3, "a_Position" },
			{ E_ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexBuffer(indexBuffer);

		m_squareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ E_ShaderDataType::Float3, "a_Position" }
			});
		m_squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->SetIndexBuffer(squareIB);

		// Shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec4 v_Color;

			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		// Shader
		std::string blackVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec4 v_Color;

			void main()
			{
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blackFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(0.0, 0.0, 0.0, 1.0);
			}
		)";

		m_blackShader.reset(new Shader(blackVertexSrc, blackFragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			m_camera.SetPosition({ 0.0f, 0.0f, 0.0f });
			m_camera.SetRotation(0.0f);

			Renderer::BeginScene(m_camera);

			Renderer::Submit(m_blackShader, m_squareVA);
			Renderer::Submit(m_shader, m_vertexArray);

			Renderer::EndScene();

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