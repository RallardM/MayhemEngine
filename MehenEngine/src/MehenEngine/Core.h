#pragma once

#include <memory>

#ifdef  MEHEN_PLATFORM_WINDOWS
#if MEHEN_DYNAMIC_LINK
	#ifdef MEHEN_BUILD_DLL
		#define MEHEN_API __declspec(dllexport)
	#else 
		#define MEHEN_API __declspec(dllimport)
	#endif // MEHEN_BUILD_DLL
#else
	#define MEHEN_API
#endif // MEHEN_DYNAMIC_LINK
#else
	#error MehenEngine only supports Windows!
#endif //  MEHEN_PLATFORM_WINDOWS

#ifdef MEHEN_DEBUG
	#define MEHEN_ENABLE_ASSERTS
#endif // MEHEN_DEBUG

// Tutorial : https://youtu.be/88dmtleVywk?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&t=1218
#ifdef MEHEN_ENABLE_ASSERTS
	#define MEHEN_GAME_ASSERT(x, ...) { if(!(x)) { MEHEN_GAME_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MEHEN_ENGINE_ASSERT(x, ...) { if(!(x)) { MEHEN_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MEHEN_GAME_ASSERT(x, ...)
	#define MEHEN_ENGINE_ASSERT(x, ...)
#endif // MEHEN_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define MEHEN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace MehenEngine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}