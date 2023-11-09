#pragma once

namespace MehenEngine
{
	enum class E_RendererAPI : unsigned __int8
	{
		None = 0,
		OpenGL = 1,
	};

	class Renderer
	{

	public:
		inline static E_RendererAPI GetAPI() { return s_RendererAPI; }

	private:
		static E_RendererAPI s_RendererAPI;

	};
}