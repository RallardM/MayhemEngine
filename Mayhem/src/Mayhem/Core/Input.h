// Input polling tutorial : https://youtu.be/yuhNj8yGDJQ?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#pragma once

#include "Base.h"
#include "MouseCodes.h"
#include "KeyCodes.h"

namespace Mayhem
{
	class Input
	{

	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(KeyCode keycode) { return _Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(MouseCode button) { return _Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return _Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return _Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return _Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual std::pair<float, float>  GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> _Instance;

	};
}