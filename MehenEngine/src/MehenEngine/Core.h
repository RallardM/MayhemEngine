#pragma once

#ifdef  MHN_PLATFORM_WINDOWS
	#ifdef MHN_BUILD_DLL
		#define MHN_API __declspec(dllexport)
	#else 
		#define MHN_API __declspec(dllimport)
	#endif // MHN_BUILD_DLL
#else
	#error MehenEngine only supports Windows!
#endif //  MHN_PLATFORM_WINDOWS

// Tutorial : https://youtu.be/88dmtleVywk?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&t=1218
#ifdef MHN_ENABLE_ASSERTS
	#define MHN_ASSERT(x, ...) { if(!(x)) { MHN_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MHN_ENGINE_ASSERT(x, ...) { if(!(x)) { MHN_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MHN_ASSERT(x, ...)
	#define MHN_ENGINE_ASSERT(x, ...)
#endif // MHN_ENABLE_ASSERTS

#define BIT(x) (1 << x)