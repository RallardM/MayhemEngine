#include <MehenEngine.h>

class ExampleLayer : public MehenEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{

	}

	void OnUpdate() override
	{
		MEHEN_GAME_INFO("ExampleLayer::Update");
	}

	void OnEvent(MehenEngine::Event& event) override
	{
		MEHEN_GAME_TRACE("{0}", event);
	}
};

class Sandbox : public MehenEngine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

MehenEngine::Application* MehenEngine::CreateApplication()
{
	return new Sandbox();
}