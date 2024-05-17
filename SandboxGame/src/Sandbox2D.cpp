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
	MAYHEM_PROFILE_FUNCTION();

	m_checkerboardTexture = Mayhem::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	MAYHEM_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Mayhem::Timestep deltaTime)
{
	MAYHEM_PROFILE_FUNCTION();

	// Update
	m_cameraController.OnUpdate(deltaTime);

	// Render
	Mayhem::Renderer2D::ResetStats();
	{
		MAYHEM_PROFILE_SCOPE("Renderer Preparation");
		Mayhem::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Mayhem::RenderCommand::Clear();
	}

	// Renderer Draw
	{
		static float rotation = 0.0f;
		rotation += deltaTime * 50.0f;

		MAYHEM_PROFILE_SCOPE("Renderer Draw");
		//Mayhem::Renderer2D::BeginScene(m_cameraController.GetCamera());
		////Mayhem::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		////Mayhem::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(-45.0f), { 0.8f, 0.2f, 0.3f, 1.0f });
		////Mayhem::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		////Mayhem::Renderer2D::DrawQuad({ -5.0f, -5.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture, 10.0f);
		//Mayhem::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Mayhem::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Mayhem::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		////Mayhem::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_checkerboardTexture, 10.0f);
		//Mayhem::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_checkerboardTexture, 20.0f);
		//Mayhem::Renderer2D::EndScene();
		Mayhem::Renderer2D::BeginScene(m_cameraController.GetCamera());
		Mayhem::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		Mayhem::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Mayhem::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Mayhem::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_checkerboardTexture, 10.0f);
		Mayhem::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_checkerboardTexture, 20.0f);
		Mayhem::Renderer2D::EndScene();

		Mayhem::Renderer2D::BeginScene(m_cameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				Mayhem::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		Mayhem::Renderer2D::EndScene();
	}

	// TODO: Add these functions: Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->Bind();
	//std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->UploadUniformFloat4("u_Color", m_squareColor);
}

void Sandbox2D::OnImGuiRender()
{
	//MAYHEM_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Mayhem::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));



	//for (auto& result : m_profileResults)
	//{
	//	char label[50];
	//	strcpy(label, "%.3fms ");
	//	strcat(label, result.Name);
	//	ImGui::Text(label, result.Time);
	//}

	//m_profileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Mayhem::Event& event)
{
	m_cameraController.OnEvent(event);
}