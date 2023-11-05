// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
// Current formatting "%^[%T] %n: %v%$"
// %^ - start color severity level
// [%T] - time stamp
// %n - name of logger (Core or Client)
// %v - message
// %$ - end color


#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace MehenEngine
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("ENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("GAME");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}