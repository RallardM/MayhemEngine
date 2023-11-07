-- Macro Tokens : https://github.com/premake/premake-core/wiki/Tokens
-- "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- %{cfg.buildcfg} - Debug, Release, Dist
-- %{cfg.system} - windows,
-- %{cfg.architecture} - x64

workspace "MehenEngine"
	architecture "x64"
	startproject "SandboxGame"

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
IncludeDir["Glad"] = "MehenEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "MehenEngine/vendor/imgui"
IncludeDir["glm"] = "MehenEngine/vendor/glm"

group "Dependencies"
		include "MehenEngine/vendor/GLFW"
		include "MehenEngine/vendor/Glad"
		include "MehenEngine/vendor/imgui"
group ""

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "Off"
		systemversion "latest"
		
		defines
		{
			"MHN_PLATFORM_WINDOWS",
			"MHN_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/SandboxGame/\"")
		}

	filter "configurations:Debug"
		defines "MHN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MHN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MHN_DIST"
		runtime "Release"
		optimize "On"

project "SandboxGame"
	location "SandboxGame"
	kind "ConsoleApp"
	language "C++"
	staticruntime "Off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"MehenEngine/vendor/spdlog/include",
		"MehenEngine/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"MehenEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		
		defines
		{
			"MHN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MHN_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MHN_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "MHN_DIST"
		runtime "Release"
		optimize "On"