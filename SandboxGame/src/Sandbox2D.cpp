#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGl/OpenGLShader.h"

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2D"),
	m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_squareVA = Mayhem::VertexArray::Create();

	float squareVertices[5 * 4] = {
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

	m_flatColorShader = Mayhem::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Mayhem::Timestep deltaTime)
{
	// Update
	m_cameraController.OnUpdate(deltaTime);

	// Render
	Mayhem::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Mayhem::RenderCommand::Clear();

	Mayhem::Renderer::BeginScene(m_cameraController.GetCamera());

	std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->Bind();
	std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->UploadUniformFloat4("u_Color", m_squareColor);

	Mayhem::Renderer::Submit(m_flatColorShader, m_squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Mayhem::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Mayhem::Event& e)
{
	m_cameraController.OnEvent(e);
}