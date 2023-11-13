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

#include <Mayhem.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Mayhem::Layer
{
public:
	ExampleLayer() :
		Layer("Example"),
		m_camera(-1.6f, 1.6f, -0.9f, 0.9f),
		m_cameraPosition(0.0f),
		m_squarePosition(0.0f)
	{
		m_vertexArray.reset(Mayhem::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.1f, 0.1f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.1f, 0.15f, 0.25f, 1.0f
		};

		Mayhem::Ref<Mayhem::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Mayhem::VertexBuffer::Create(vertices, sizeof(vertices)));

		Mayhem::BufferLayout layout = {
			{ Mayhem::E_ShaderDataType::Float3, "a_Position" },
			{ Mayhem::E_ShaderDataType::Float4, "a_Color"}
		};

		vertexBuffer->SetLayout(layout);
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Mayhem::Ref<Mayhem::IndexBuffer> indexBuffer;
		indexBuffer.reset(Mayhem::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexBuffer(indexBuffer);

		m_squareVA.reset(Mayhem::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		Mayhem::Ref<Mayhem::VertexBuffer> squareVB;
		squareVB.reset(Mayhem::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Mayhem::E_ShaderDataType::Float3, "a_Position" }
			});
		m_squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Mayhem::Ref<Mayhem::IndexBuffer> squareIB;
		squareIB.reset(Mayhem::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_squareVA->SetIndexBuffer(squareIB);

		// Texture

		m_textureVA.reset(Mayhem::VertexArray::Create());

		float textureVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f ,0.0f, 1.0f
		};

		Mayhem::Ref<Mayhem::VertexBuffer> m_textureVB;
		m_textureVB.reset(Mayhem::VertexBuffer::Create(textureVertices, sizeof(textureVertices)));

		m_textureVB->SetLayout({
			{ Mayhem::E_ShaderDataType::Float3, "a_Position" },
			{ Mayhem::E_ShaderDataType::Float2, "a_TexCoord" }
			});
		m_textureVA->AddVertexBuffer(m_textureVB);

		uint32_t textureIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Mayhem::Ref<Mayhem::IndexBuffer> textureIB;
		textureIB.reset(Mayhem::IndexBuffer::Create(textureIndices, sizeof(textureIndices) / sizeof(uint32_t)));
		m_textureVA->SetIndexBuffer(textureIB);

		// Shader
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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

		m_shader.reset(Mayhem::Shader::Create(vertexSrc, fragmentSrc));

		// Shader
		std::string blackVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec4 v_Color;

			void main()
			{
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string blackFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_blackShader.reset(Mayhem::Shader::Create(blackVertexSrc, blackFragmentSrc));

		// Texture Shader
		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_textureShader.reset(Mayhem::Shader::Create(textureVertexSrc, textureFragmentSrc));
		m_texture = Mayhem::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_textureShader)->Bind();
		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Mayhem::Timestep deltaTime) override
	{
		MAYHEM_GAME_TRACE("Delta time: {0}s ({1}ms)", deltaTime.GetSeconds(), deltaTime.GetMilliseconds());

		// Move camera
		if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_A))
		{
			m_cameraPosition.x -= m_cameraMoveSpeed * deltaTime;
		}
		else if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_D))
		{
			m_cameraPosition.x += m_cameraMoveSpeed * deltaTime;
		}
		if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_W))
		{
			m_cameraPosition.y += m_cameraMoveSpeed * deltaTime;
		}
		else if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_S))
		{
			m_cameraPosition.y -= m_cameraMoveSpeed * deltaTime;
		}
		if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_Q))
		{
			m_cameraRotation += m_cameraRotationSpeed * deltaTime;
		}
		else if (Mayhem::Input::IsKeyPressed(MAYHEM_KEY_E))
		{
			m_cameraRotation -= m_cameraRotationSpeed * deltaTime;
		}

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

		Mayhem::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Mayhem::RenderCommand::Clear();

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_cameraRotation);

		Mayhem::Renderer::BeginScene(m_camera);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_squarePosition)
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_squareRotation), glm::vec3(0, 0, 1));


		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_blackShader)->Bind();
		std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_blackShader)->UploadUniformFloat3("u_Color", m_squareColor);

		m_texture->Bind();

		Mayhem::Renderer::Submit(m_blackShader, m_squareVA, transform);
		Mayhem::Renderer::Submit(m_shader, m_vertexArray);
		Mayhem::Renderer::Submit(m_textureShader, m_textureVA);
		

		Mayhem::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_squareColor));
		ImGui::End();
	}

	void OnEvent(Mayhem::Event& event) override
	{

	}

private:
	// Triangle
	Mayhem::Ref<Mayhem::Shader> m_shader;
	Mayhem::Ref<Mayhem::VertexArray> m_vertexArray;

	// Square
	Mayhem::Ref<Mayhem::VertexArray> m_squareVA;
	Mayhem::Ref<Mayhem::Shader> m_blackShader;

	// Texture
	Mayhem::Ref<Mayhem::VertexArray> m_textureVA;
	Mayhem::Ref<Mayhem::Shader> m_textureShader;
	Mayhem::Ref<Mayhem::Texture2D> m_texture;

	Mayhem::OrthographicCamera m_camera;

	glm::vec3 m_squareColor = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_squarePosition;
	glm::vec3 m_cameraPosition;

	float m_cameraMoveSpeed = 1.0f;
	float m_squareMoveSpeed = 1.0f;
	float m_cameraRotation = 0.0f;
	float m_squareRotation = 0.0f;
	float m_cameraRotationSpeed = 100.0f;
	float m_squareRotationSpeed = 100.0f;

};

class Sandbox : public Mayhem::Application
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

Mayhem::Application* Mayhem::CreateApplication()
{
	return new Sandbox();
}