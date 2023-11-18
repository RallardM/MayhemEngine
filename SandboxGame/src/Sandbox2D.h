#pragma once

#include <Mayhem.h>

class Sandbox2D : public Mayhem::Layer
{

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Mayhem::Timestep deltaTime) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Mayhem::Event& e) override;

private:
	Mayhem::OrthographicCameraController m_cameraController;

	// Temporary
	Mayhem::Ref<Mayhem::VertexArray> m_squareVA;
	Mayhem::Ref<Mayhem::Shader> m_flatColorShader;


	glm::vec4 m_squareColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	glm::vec3 m_squarePosition = { 0.0f, 0.0f, 0.0f };

	float m_squareMoveSpeed = 1.0f;
	float m_squareRotation = 0.0f;
	float m_squareRotationSpeed = 100.0f;

};