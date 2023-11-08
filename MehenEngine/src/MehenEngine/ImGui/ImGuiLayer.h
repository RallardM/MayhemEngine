#pragma once

#include "MehenEngine/Layer.h"

#include "MehenEngine/Events/ApplicationEvent.h"
#include "MehenEngine/Events/KeyEvent.h"
#include "MehenEngine/Events/MouseEvent.h"

namespace MehenEngine
{
	class MEHEN_API ImGuiLayer : public Layer
	{

	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float m_time = 0.0f;

	};
}