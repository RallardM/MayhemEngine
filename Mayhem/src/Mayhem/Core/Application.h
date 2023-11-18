#pragma once

#include "Core.h"

#include "Window.h"
#include "Mayhem/Core/LayerStack.h"
#include "Mayhem/Events/Event.h"
#include "Mayhem/Events/ApplicationEvent.h"

#include "Mayhem/Core/Timestep.h"

#include "Mayhem/ImGui/ImGuiLayer.h"

namespace Mayhem
{
	class Application
	{

	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_window; }
		inline static Application& Get() { return *_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		static Application* _Instance;
		Ref<Window> m_window;
		ImGuiLayer* m_imguiLayer;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
		bool m_running = true;
		bool m_minimized = false;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}