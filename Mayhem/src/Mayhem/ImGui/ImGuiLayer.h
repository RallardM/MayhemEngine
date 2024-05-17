#pragma once

#include "Mayhem/Core/Layer.h"

#include "Mayhem/Events/ApplicationEvent.h"
#include "Mayhem/Events/KeyEvent.h"
#include "Mayhem/Events/MouseEvent.h"

namespace Mayhem
{
	class ImGuiLayer : public Layer
	{

	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		float m_time = 0.0f;

	};
}