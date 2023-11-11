#pragma once

#include "Core.h"

#include "Window.h"
#include "MehenEngine/LayerStack.h"
#include "MehenEngine/Events/Event.h"
#include "MehenEngine/Events/ApplicationEvent.h"

#include "MehenEngine/ImGui/ImGuiLayer.h"

// TEMPORARY
#include "MehenEngine/Renderer/Shader.h"
#include "MehenEngine/Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

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

		inline Window& GetWindow() { return *m_window; }
		inline static Application& Get() { return *_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		static Application* _Instance;

		// Triangle
		std::shared_ptr<Shader> m_shader;
		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<Window> m_window;

		// Square
		std::shared_ptr<VertexArray> m_squareVA;
		std::shared_ptr<Shader> m_blackShader;

		ImGuiLayer* m_imguiLayer;

		LayerStack m_layerStack;

		bool m_running = true;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}