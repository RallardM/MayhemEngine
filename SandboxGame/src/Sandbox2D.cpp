#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2D"),
	m_cameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
	m_checkerboardTexture = Mayhem::Texture2D::Create("assets/textures/Checkerboard.png");
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

	Mayhem::Renderer2D::BeginScene(m_cameraController.GetCamera());

	Mayhem::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	//Mayhem::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_squareColor);
	Mayhem::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	//Mayhem::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_checkerboardTexture);
	Mayhem::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture);
	
	Mayhem::Renderer2D::EndScene();

	// TODO: Add these functions: Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->Bind();
	//std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->UploadUniformFloat4("u_Color", m_squareColor);
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