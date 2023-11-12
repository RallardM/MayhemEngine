#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace MehenEngine
{
	class MEHEN_API Log
	{
	public:
		static void Init();

		inline static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define MEHEN_ENGINE_TRACE(...)	 ::MehenEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MEHEN_ENGINE_INFO(...)	 ::MehenEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MEHEN_ENGINE_WARN(...)	 ::MehenEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MEHEN_ENGINE_ERROR(...)	 ::MehenEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MEHEN_ENGINE_FATAL(...)	 ::MehenEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define MEHEN_GAME_TRACE(...)      ::MehenEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MEHEN_GAME_INFO(...)       ::MehenEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define MEHEN_GAME_WARN(...)       ::MehenEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MEHEN_GAME_ERROR(...)      ::MehenEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define MEHEN_GAME_FATAL(...)      ::MehenEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)
