-- Macro Tokens : https://github.com/premake/premake-core/wiki/Tokens
-- "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- %{cfg.buildcfg} - Debug, Release, Dist
-- %{cfg.system} - windows,
-- %{cfg.architecture} - x64

workspace "MehenEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "MehenEngine/vendor/GLFW/include"

include "MehenEngine/vendor/GLFW"

project "MehenEngine"
	location "MehenEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "MhnPCHeader.h"
	pchsource "MehenEngine/src/MhnPCHeader.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"
		
		defines
		{
			"MHN_PLATFORM_WINDOWS",
			"MHN_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SandboxGame")
		}

	filter "configurations:Debug"
		defines "MHN_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "MHN_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MHN_DIST"
		optimize "On"

project "SandboxGame"
	location "SandboxGame"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"MehenEngine/vendor/spdlog/include",
		"MehenEngine/src"
	}

	links
	{
		"MehenEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines
		{
			"MHN_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "MHN_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MHN_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "MHN_DIST"
		optimize "On"