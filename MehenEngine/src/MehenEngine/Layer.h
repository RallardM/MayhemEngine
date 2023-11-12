#pragma once

#include "MehenEngine/Core.h"
#include "MehenEngine/Core/Timestep.h"
#include "MehenEngine/Events/Event.h"

namespace MehenEngine
{
	class MEHEN_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }
	protected:
		std::string m_debugName;
	};
}