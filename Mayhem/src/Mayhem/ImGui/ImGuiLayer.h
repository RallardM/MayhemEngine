#pragma once

#include "Mayhem/Layer.h"

#include "Mayhem/Events/ApplicationEvent.h"
#include "Mayhem/Events/KeyEvent.h"
#include "Mayhem/Events/MouseEvent.h"

namespace Mayhem
{
	class MAYHEM_API ImGuiLayer : public Layer
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