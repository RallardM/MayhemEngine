#pragma once

#include "Core.h"

#include "Window.h"
#include "MehenEngine/LayerStack.h"
#include "MehenEngine/Events/Event.h"
#include "MehenEngine/Events/ApplicationEvent.h"

#include "MehenEngine/ImGui/ImGuiLayer.h"

// TEMPORARY
#include "MehenEngine/Renderer/Shader.h"

namespace MehenEngine
{
	class MEHEN_API Application
	{

	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *_Instance; }
		inline Window& GetWindow() { return *m_window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		static Application* _Instance;

		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imguiLayer;

		LayerStack m_layerStack;
		unsigned int m_vertexArray, m_vertexBuffer, m_indexBuffer;
		bool m_running = true;

		std::unique_ptr<Shader> m_shader;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}