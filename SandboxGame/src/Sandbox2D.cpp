#include "Sandbox2D.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, const Fn& func)
		: m_name(name), m_func(func), m_stopped(false)
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_stopped)
			Stop();
	}

	void Stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTime).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

		m_stopped = true;

		auto duration = end - start;
		float ms = duration * 0.001;

		m_func({ m_name, ms });

		std::cout << m_name << ": " << ms << "ms" << std::endl;


		//std::cout << "Duration: " << duration << "ms" << std::endl;
	}

private:
	const char* m_name;
	Fn m_func;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	bool m_stopped;

};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_profileResults.push_back(profileResult); })

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
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	// Update
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_cameraController.OnUpdate(deltaTime);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Preparation");
		Mayhem::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Mayhem::RenderCommand::Clear();
	}

	// Renderer Draw
	{

		Mayhem::Renderer2D::BeginScene(m_cameraController.GetCamera());

		Mayhem::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		//Mayhem::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_squareColor);
		Mayhem::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		//Mayhem::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_checkerboardTexture);
		Mayhem::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_checkerboardTexture);

		Mayhem::Renderer2D::EndScene();

	}

	// TODO: Add these functions: Shader::SetMat4, Shader::SetFloat4
	//std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->Bind();
	//std::dynamic_pointer_cast<Mayhem::OpenGLShader>(m_flatColorShader)->UploadUniformFloat4("u_Color", m_squareColor);
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareColor));

	for (auto& result : m_profileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}

	m_profileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Mayhem::Event& e)
{
	m_cameraController.OnEvent(e);
}