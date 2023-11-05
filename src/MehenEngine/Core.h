#pragma once

#ifdef  MHE_PLATFORM_WINDOWS
	#ifdef MHE_BUILD_DLL
		#define MEHEN_ENGINE_API __declspec(dllexport)
	#else 
		#define MEHEN_ENGINE_API __declspec(dllimport)
	#endif // MHE_BUILD_DLL
#else
	#error MehenEngine only supports Windows!
#endif //  MHE_PLATFORM_WINDOWS
