#pragma once

#include <memory>

#ifdef  MAYHEM_PLATFORM_WINDOWS
#if MAYHEM_DYNAMIC_LINK
	#ifdef MAYHEM_BUILD_DLL
		#define MAYHEM_API __declspec(dllexport)
	#else 
		#define MAYHEM_API __declspec(dllimport)
	#endif // MAYHEM_BUILD_DLL
#else
	#define MAYHEM_API
#endif // MAYHEM_DYNAMIC_LINK
#else
	#error Mayhem only supports Windows!
#endif //  MAYHEM_PLATFORM_WINDOWS

#ifdef MAYHEM_DEBUG
	#define MAYHEM_ENABLE_ASSERTS
#endif // MAYHEM_DEBUG

// Tutorial : https://youtu.be/88dmtleVywk?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&t=1218
#ifdef MAYHEM_ENABLE_ASSERTS
	#define MAYHEM_GAME_ASSERT(x, ...) { if(!(x)) { MAYHEM_GAME_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MAYHEM_ENGINE_ASSERT(x, ...) { if(!(x)) { MAYHEM_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MAYHEM_GAME_ASSERT(x, ...)
	#define MAYHEM_ENGINE_ASSERT(x, ...)
#endif // MAYHEM_ENABLE_ASSERTS

#define BIT(x) (1 << x)

#define MAYHEM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Mayhem
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}