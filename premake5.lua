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

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" 

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "MehenEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "MehenEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "MehenEngine/vendor/imgui"
IncludeDir["glm"] = "MehenEngine/vendor/glm"
IncludeDir["stb_image"] = "MehenEngine/vendor/stb_image"

group "Dependencies"
		include "MehenEngine/vendor/GLFW"
		include "MehenEngine/vendor/Glad"
		include "MehenEngine/vendor/imgui"
group ""

-- Main Project --------------------------------------------------
project "MehenEngine"
	location "MehenEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "MehenPrecompiledHeaders.h"
	pchsource "MehenEngine/src/MehenPrecompiledHeaders.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

-- Platforms -----------------------------------------------------
	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"MEHEN_PLATFORM_WINDOWS",
			"MEHEN_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "MEHEN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MEHEN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MEHEN_DIST"
		runtime "Release"
		optimize "on"

-- Game Project -----------------------------------------------------
project "SandboxGame"
	location "SandboxGame"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"MehenEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"MehenEngine"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"MEHEN_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MEHEN_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MEHEN_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MEHEN_DIST"
		runtime "Release"
		optimize "on"