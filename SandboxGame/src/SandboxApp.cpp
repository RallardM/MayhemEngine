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

#include <MehenEngine.h>

#include "imgui/imgui.h"

class ExampleLayer : public MehenEngine::Layer
{
public:
	ExampleLayer() :
		Layer("Example"),
		m_camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_cameraPosition(0.0f)
	{
		m_vertexArray.reset(MehenEngine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.1f, 0.15f, 0.25f, 1.0f
		};

		std::shared_ptr<MehenEngine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(MehenEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		MehenEngine::BufferLayout layout = {
			{ MehenEngine::E_ShaderDataType::Float3, "a_Position" },
			{ MehenEngine::E_ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<MehenEngine::IndexBuffer> indexBuffer;
		indexBuffer.reset(MehenEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexBuffer(indexBuffer);

		m_squareVA.reset(MehenEngine::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<MehenEngine::VertexBuffer> squareVB;
		squareVB.reset(MehenEngine::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ MehenEngine::E_ShaderDataType::Float3, "a_Position" }
			});
		m_squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<MehenEngine::IndexBuffer> squareIB;
		squareIB.reset(MehenEngine::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_shader.reset(new MehenEngine::Shader(vertexSrc, fragmentSrc));

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

		m_blackShader.reset(new MehenEngine::Shader(blackVertexSrc, blackFragmentSrc));
	}

	void OnUpdate(MehenEngine::Timestep deltaTime) override
	{
		MEHEN_GAME_TRACE("Delta time: {0}s ({1}ms)", deltaTime.GetSeconds(), deltaTime.GetMilliseconds());

		if (MehenEngine::Input::IsKeyPressed(MEHEN_KEY_A))
		{
			m_cameraPosition.x -= m_cameraSpeed * deltaTime;
		}
		else if (MehenEngine::Input::IsKeyPressed(MEHEN_KEY_D))
		{
			m_cameraPosition.x += m_cameraSpeed * deltaTime;
		}
		if (MehenEngine::Input::IsKeyPressed(MEHEN_KEY_W))
		{
			m_cameraPosition.y += m_cameraSpeed * deltaTime;
		}
		else if (MehenEngine::Input::IsKeyPressed(MEHEN_KEY_S))
		{
			m_cameraPosition.y -= m_cameraSpeed * deltaTime;
		}
		if (MehenEngine::Input::IsKeyPressed(MEHEN_KEY_Q))
		{
			m_cameraRotation += m_cameraRotationSpeed * deltaTime;
		}
		else if (MehenEngine::Input::IsKeyPressed(MEHEN_KEY_E))
		{
			m_cameraRotation -= m_cameraRotationSpeed * deltaTime;
		}

		MehenEngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		MehenEngine::RenderCommand::Clear();

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_cameraRotation);

		MehenEngine::Renderer::BeginScene(m_camera);

		MehenEngine::Renderer::Submit(m_blackShader, m_squareVA);
		MehenEngine::Renderer::Submit(m_shader, m_vertexArray);

		MehenEngine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{

	}

	void OnEvent(MehenEngine::Event& event) override
	{

	}

private:
	// Triangle
	std::shared_ptr<MehenEngine::Shader> m_shader;
	std::shared_ptr<MehenEngine::VertexArray> m_vertexArray;

	// Square
	std::shared_ptr<MehenEngine::VertexArray> m_squareVA;
	std::shared_ptr<MehenEngine::Shader> m_blackShader;

	MehenEngine::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraSpeed = 1.0f;
	float m_cameraRotation = 0.0f;
	float m_cameraRotationSpeed = 100.0f;

};

class Sandbox : public MehenEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

MehenEngine::Application* MehenEngine::CreateApplication()
{
	return new Sandbox();
}