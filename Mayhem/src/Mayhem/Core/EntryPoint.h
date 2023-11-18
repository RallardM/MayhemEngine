#pragma once

#ifdef  MAYHEM_PLATFORM_WINDOWS

extern Mayhem::Application* Mayhem::CreateApplication();

int main(int argc, char** argv)
{
	Mayhem::Log::Init();
	MAYHEM_ENGINE_WARN("Initialized Log!");
	int a = 5;
	MAYHEM_GAME_INFO("Hello! Var={0}", a);

	auto app = Mayhem::CreateApplication();
	app->Run();
	delete app;
}

#endif