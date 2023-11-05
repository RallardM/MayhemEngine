#pragma once

#ifdef  MHN_PLATFORM_WINDOWS
	#ifdef MHN_BUILD_DLL
		#define MHN_ENGINE_API __declspec(dllexport)
	#else 
		#define MHN_ENGINE_API __declspec(dllimport)
	#endif // MHN_BUILD_DLL
#else
	#error MehenEngine only supports Windows!
#endif //  MHN_PLATFORM_WINDOWS
