#pragma once

#include "MehenEngine/Layer.h"

namespace MehenEngine
{
	class MEHEN_API ImGuiLayer : public Layer
	{

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);

	private:
		float m_time = 0.0f;

	};
}