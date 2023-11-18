/*
 * Copyright 2023 TheCherno/Mayhem
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

#include <Mayhem.h>
#include <Mayhem/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer : public Mayhem::Layer
{
public:
	ExampleLayer() :
		Layer("Example"),
		m_cameraController(1280.0f / 720.0f, true)
	{
		m_vertexArray = Mayhem::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Mayhem::Ref<Mayhem::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mayhem::VertexBuffer::Create(vertices, sizeof(vertices)));
		Mayhem::BufferLayout layout = {
			{ Mayhem::E_ShaderDataType::Float3, "a_Position" },
			{ Mayhem::E_ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Mayhem::Ref<Mayhem::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mayhem::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexBuffer(indexBuffer);

		m_squareVA = Mayhem::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Mayhem::Ref<Mayhem::VertexBuffer> squareVB;
		squareVB.reset(Mayhem::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Mayhem::E_ShaderDataType::Float3, "a_Position" },
			{ Mayhem::E_ShaderDataType::Float2, "a_TexCoord" }
			});
		m_squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Mayhem::Ref<Mayhem::IndexBuffer> squareIB;
		squareIB.reset(Mayhem::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_shader = Mayhem::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_blackShader = Mayhem::Shader::Create("flatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		auto textureShader = m_shaderLibrary.Load("assets/shaders/TextureShader.glsl");

		m_texture = Mayhem::Texture2D::Create("assets/textures/Checkerboard.png");
		m_alphaTexture = Mayhem::Texture2D::Create("assets/textures/re_and_mehen.png");

		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Mayhem::Timestep deltaTime) override
	{
		//MAYHEM_GAME_TRACE("Delta time: {0}s ({1}ms)", deltaTime.GetSeconds(), deltaTime.GetMilliseconds());

		// Update
		m_cameraController.OnUpdate(deltaTime);

		// Move square
		if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_LEFT))
		{
			m_squarePosition.x -= m_squareMoveSpeed * deltaTime;
		}
		else if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_RIGHT))
		{
			m_squarePosition.x += m_squareMoveSpeed * deltaTime;
		}
		if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_UP))
		{
			m_squarePosition.y += m_squareMoveSpeed * deltaTime;
		}
		else if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_DOWN))
		{
			m_squarePosition.y -= m_squareMoveSpeed * deltaTime;
		}
		if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_PAGE_UP))
		{
			m_squareRotation += m_squareRotationSpeed * deltaTime;
		}
		else if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_PAGE_DOWN))
		{
			m_squareRotation -= m_squareRotationSpeed * deltaTime;
		}

		// Render
		Mayhem::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Mayhem::RenderCommand::Clear();

		Mayhem::Renderer::BeginScene(m_cameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_blackShader)->Bind();
		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_blackShader)->UploadUniformFloat3("u_Color", m_squareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Mayhem::Renderer::Submit(m_blackShader, m_squareVA, transform);
			}
		}

		auto textureShader = m_shaderLibrary.Get("TextureShader");

		m_texture->Bind();
		Mayhem::Renderer::Submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_alphaTexture->Bind();
		Mayhem::Renderer::Submit(textureShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


		// Triangle
		// Mayhem::Renderer::Submit(m_shader, m_vertexArray);

		Mayhem::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}

	void OnEvent(Mayhem::Event& e) override
	{
		m_cameraController.OnEvent(e);
	}

private:
	Mayhem::ShaderLibrary m_shaderLibrary;

	// Triangle
	Mayhem::Ref<Mayhem::Shader> m_shader;
	Mayhem::Ref<Mayhem::VertexArray> m_vertexArray;

	// Square
	Mayhem::Ref<Mayhem::VertexArray> m_squareVA;
	Mayhem::Ref<Mayhem::Shader> m_blackShader;

	// Texture
	Mayhem::Ref<Mayhem::VertexArray> m_textureVA;

	Mayhem::Ref<Mayhem::Texture2D> m_texture, m_alphaTexture;

	Mayhem::OrthographicCameraController m_cameraController;

	glm::vec3 m_squareColor = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_squarePosition = { 0.0f, 0.0f, 0.0f };

	float m_squareMoveSpeed = 1.0f;
	float m_squareRotation = 0.0f;
	float m_squareRotationSpeed = 100.0f;

};

class Sandbox : public Mayhem::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}
};

Mayhem::Application* Mayhem::CreateApplication()
{
	return new Sandbox();
}