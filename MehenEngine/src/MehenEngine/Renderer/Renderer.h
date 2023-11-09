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
		inline static E_RendererAPI GetAPI() const { return m_API; }
	private:
		inline E_RendererAPI SetAPI(E_RendererAPI api) { m_API = api; }
	};
}