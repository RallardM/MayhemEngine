#include "Application.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "MehenEngine/Log.h"

namespace MehenEngine
{
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent resizeEvent(1280, 720);
		MHN_GAME_TRACE(resizeEvent);

		while (true);
	}
}