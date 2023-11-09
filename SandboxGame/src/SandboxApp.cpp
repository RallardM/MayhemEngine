/*
 * Copyright 2023 TheCherno/Hazel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <MehenEngine.h>

#include "imgui/imgui.h"

class ExampleLayer : public MehenEngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (MehenEngine::Input::IsKeyPressed(MEHEN_KEY_TAB))
		{
			MEHEN_GAME_TRACE("Tab key is pressed (poll)!");
		}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
	}

	void OnEvent(MehenEngine::Event& event) override
	{
		if (event.GetEventType() == MehenEngine::E_EventType::KeyPressed)
		{

			MehenEngine::KeyPressedEvent& e = (MehenEngine::KeyPressedEvent&)event;

			if (e.GetKeyCode() == MEHEN_KEY_TAB)
			{
				MEHEN_GAME_TRACE("Tab key is pressed (event)!");
			}

			MEHEN_GAME_TRACE("{0}", (char)e.GetKeyCode());
		}
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