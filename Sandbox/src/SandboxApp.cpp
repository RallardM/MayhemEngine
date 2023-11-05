#include <MehenEngine.h>

class Sandbox : public MehenEngine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

MehenEngine::Application* MehenEngine::CreateApplication()
{
	return new Sandbox();
}