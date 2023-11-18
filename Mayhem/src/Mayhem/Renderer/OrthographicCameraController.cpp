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

 // Tutorial : https://youtu.be/q7rwMoZQSmw?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#include "MayhemPrecompiledHeaders.h"
#include "OrthographicCameraController.h"

#include "Mayhem/Core/Input.h"
#include "Mayhem/Core/KeyCodes.h"

namespace Mayhem
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		m_aspectRatio(aspectRatio),
		m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel),
		m_rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(float deltaTime, bool rotation)
	{
		// Move camera
		if (Input::IsKeyPressed(MAYHEM_KEY_A))
		{
			m_cameraPosition.x -= m_cameraTrasnslationSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(MAYHEM_KEY_D))
		{
			m_cameraPosition.x += m_cameraTrasnslationSpeed * deltaTime;
		}
		if (Input::IsKeyPressed(MAYHEM_KEY_W))
		{
			m_cameraPosition.y += m_cameraTrasnslationSpeed * deltaTime;
		}
		else if (Input::IsKeyPressed(MAYHEM_KEY_S))
		{
			m_cameraPosition.y -= m_cameraTrasnslationSpeed * deltaTime;
		}

		// Rotate camera
		if (m_rotation)
		{
			if (Input::IsKeyPressed(MAYHEM_KEY_Q))
			{
				m_cameraRotation += m_cameraRotationSpeed * deltaTime;
			}
			else if (Input::IsKeyPressed(MAYHEM_KEY_E))
			{
				m_cameraRotation -= m_cameraRotationSpeed * deltaTime;
			}

			m_camera.SetRotation(m_cameraRotation);
		}

		m_camera.SetPosition(m_cameraPosition);

		m_cameraTrasnslationSpeed = m_zoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseScrolledEvent>(MAYHEM_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(MAYHEM_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
	{
		m_zoomLevel -= event.GetYOffset() * 0.25f;
		m_zoomLevel = std::max(m_zoomLevel, 0.25f);
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
	{
		m_aspectRatio = (float)event.GetWidth() / (float)event.GetHeight();
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}
}