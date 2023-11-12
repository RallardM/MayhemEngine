#pragma once

#include "Core.h"

#include "Window.h"
#include "MehenEngine/LayerStack.h"
#include "MehenEngine/Events/Event.h"
#include "MehenEngine/Events/ApplicationEvent.h"

#include "MehenEngine/Core/Timestep.h"

#include "MehenEngine/ImGui/ImGuiLayer.h"

namespace MehenEngine
{
	class Application
	{

	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_window; }
		inline static Application& Get() { return *_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		static Application* _Instance;
		std::shared_ptr<Window> m_window;
		ImGuiLayer* m_imguiLayer;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
		bool m_running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}