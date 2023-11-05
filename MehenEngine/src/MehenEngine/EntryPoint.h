#pragma once

#ifdef  MHE_PLATFORM_WINDOWS

extern MehenEngine::Application* MehenEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = MehenEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif