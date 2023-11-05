#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace MehenEngine
{
	class MHN_ENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define MHN_ENGINE_TRACE(...) ::MehenEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MHN_ENGINE_INFO(...)  ::MehenEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MHN_ENGINE_WARN(...)  ::MehenEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MHN_ENGINE_ERROR(...) ::MehenEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MHN_ENGINE_FATAL(...) ::MehenEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define MHN_GAME_TRACE(...)      ::MehenEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MHN_GAME_INFO(...)       ::MehenEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define MHN_GAME_WARN(...)       ::MehenEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MHN_GAME_ERROR(...)      ::MehenEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define MHN_GAME_FATAL(...)      ::MehenEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)
