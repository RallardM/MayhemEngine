#pragma once

#ifdef  MAYHEM_PLATFORM_WINDOWS

extern Mayhem::Application* Mayhem::CreateApplication();

int main(int argc, char** argv)
{
	Mayhem::Log::Init();

	MAYHEM_PROFILE_BEGIN_SESSION("Startup", "MayhemProfile-Startup.json");
	auto app = Mayhem::CreateApplication();
	MAYHEM_PROFILE_END_SESSION();

	MAYHEM_PROFILE_BEGIN_SESSION("Runtime", "MayhemProfile-Runtime.json");
	app->Run();
	MAYHEM_PROFILE_END_SESSION();

	MAYHEM_PROFILE_BEGIN_SESSION("Shutdown", "MayhemProfile-Shutdown.json");
	delete app;
	MAYHEM_PROFILE_END_SESSION();
}

#endif