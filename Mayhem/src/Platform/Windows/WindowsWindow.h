#pragma once

#include "Mayhem/Core/Window.h"
#include "Mayhem/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Mayhem
{
	class WindowsWindow : public Window
	{

	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_data.Width; }
		inline unsigned int GetHeight() const override { return m_data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_window; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_window;
		Scope<GraphicsContext> m_context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		WindowData m_data;

	};
}