#pragma once

#include "Mayhem/Renderer/OrthographicCamera.h"
#include "Mayhem/Core/Timestep.h"

#include "Mayhem/Events/ApplicationEvent.h"
#include "Mayhem/Events/MouseEvent.h"

namespace Mayhem
{
	class OrthographicCameraController
	{

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(float deltaTime, bool rotation = false);
		void OnEvent(Event& event);

		OrthographicCamera& GetCamera() { return m_camera; }
		const OrthographicCamera& GetCamera() const { return m_camera; }

		//float GetZoomLevel() const { return m_zoomLevel; }
		//void SetZoomLevel(float level) { m_zoomLevel = level; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& event);
		bool OnWindowResized(WindowResizeEvent& event);

		float m_aspectRatio;
		float m_zoomLevel = 1.0f;
		OrthographicCamera m_camera;

		bool m_rotation;
		glm::vec3 m_cameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_cameraRotation = 0.0f;
		float m_cameraTrasnslationSpeed = 1.0f;
		float m_cameraRotationSpeed = 100.0f;

	};
}