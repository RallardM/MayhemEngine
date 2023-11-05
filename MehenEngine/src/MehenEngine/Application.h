#pragma once

#include "Core.h"

namespace MehenEngine
{
	class MHN_ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	};

	Application* CreateApplication();

}