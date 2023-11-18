#pragma once

#include "Mayhem/Core/Core.h"
#include "Mayhem/Core/Timestep.h"
#include "Mayhem/Events/Event.h"

namespace Mayhem
{
	class MAYHEM_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

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