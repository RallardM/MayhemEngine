// Input polling tutorial : https://youtu.be/yuhNj8yGDJQ?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

#pragma once

#include "Mayhem/Core/Core.h"

namespace Mayhem
{
	class MAYHEM_API Input
	{

	protected:
		Input() = default;

	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(int keycode) { return _Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return _Instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<float, float> GetMousePosition() { return _Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return _Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return _Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float>  GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:
		static Scope<Input> _Instance;

	};
}