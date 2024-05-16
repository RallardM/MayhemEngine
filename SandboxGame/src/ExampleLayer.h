#pragma once

#include "Mayhem.h"

class ExampleLayer : public Mayhem::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Mayhem::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Mayhem::Event& e) override;


private:
	Mayhem::ShaderLibrary m_shaderLibrary;

	Mayhem::Ref<Mayhem::Shader> m_flatColorShader;

	// Triangle
	Mayhem::Ref<Mayhem::Shader> m_shader;
	Mayhem::Ref<Mayhem::VertexArray> m_vertexArray;

	// Square
	Mayhem::Ref<Mayhem::VertexArray> m_squareVA;
	Mayhem::Ref<Mayhem::Shader> m_blackShader;

	// Texture
	Mayhem::Ref<Mayhem::VertexArray> m_textureVA;

	Mayhem::Ref<Mayhem::Texture2D> m_texture, m_alphaTexture, m_mayhemLogoTexture;;

	Mayhem::OrthographicCameraController m_cameraController;

	//glm::vec3 m_squareColor = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_squareColor = { 0.2f, 0.3f, 0.8f };
	glm::vec3 m_squarePosition = { 0.0f, 0.0f, 0.0f };

	float m_squareMoveSpeed = 1.0f;
	float m_squareRotation = 0.0f;
	float m_squareRotationSpeed = 100.0f;

};