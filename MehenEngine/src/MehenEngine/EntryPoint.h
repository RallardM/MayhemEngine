#pragma once

#ifdef  MHN_PLATFORM_WINDOWS

extern MehenEngine::Application* MehenEngine::CreateApplication();

int main(int argc, char** argv)
{
	MehenEngine::Log::Init();
	MHN_ENGINE_WARN("Initialized Log!");
	int a = 5;
	MHN_GAME_INFO("Hello! Var={0}", a);

	auto app = MehenEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif