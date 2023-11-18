#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Mayhem
{
	class MAYHEM_API Log
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
#define MAYHEM_ENGINE_TRACE(...)	 ::Mayhem::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MAYHEM_ENGINE_INFO(...)	 ::Mayhem::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MAYHEM_ENGINE_WARN(...)	 ::Mayhem::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MAYHEM_ENGINE_ERROR(...)	 ::Mayhem::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MAYHEM_ENGINE_FATAL(...)	 ::Mayhem::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MAYHEM_GAME_TRACE(...)      ::Mayhem::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MAYHEM_GAME_INFO(...)       ::Mayhem::Log::GetClientLogger()->info(__VA_ARGS__)
#define MAYHEM_GAME_WARN(...)       ::Mayhem::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MAYHEM_GAME_ERROR(...)      ::Mayhem::Log::GetClientLogger()->error(__VA_ARGS__)
#define MAYHEM_GAME_FATAL(...)      ::Mayhem::Log::GetClientLogger()->critical(__VA_ARGS__)