-- Macro Tokens : https://github.com/premake/premake-core/wiki/Tokens
-- "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
-- %{cfg.buildcfg} - Debug, Release, Dist
-- %{cfg.system} - windows,
-- %{cfg.architecture} - x64

workspace "Mayhem"
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
IncludeDir["GLFW"] = "Mayhem/vendor/GLFW/include"
IncludeDir["Glad"] = "Mayhem/vendor/Glad/include"
IncludeDir["ImGui"] = "Mayhem/vendor/imgui"
IncludeDir["glm"] = "Mayhem/vendor/glm"
IncludeDir["stb_image"] = "Mayhem/vendor/stb_image"

group "Dependencies"
		include "Mayhem/vendor/GLFW"
		include "Mayhem/vendor/Glad"
		include "Mayhem/vendor/imgui"
group ""

-- Main Project --------------------------------------------------
project "Mayhem"
	location "Mayhem"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "MayhemPrecompiledHeaders.h"
	pchsource "Mayhem/src/MayhemPrecompiledHeaders.cpp"

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
			"MAYHEM_PLATFORM_WINDOWS",
			"MAYHEM_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "MAYHEM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MAYHEM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MAYHEM_DIST"
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
		"Mayhem/vendor/spdlog/include",
		"Mayhem/src",
		"Mayhem/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Mayhem"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"MAYHEM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "MAYHEM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MAYHEM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "MAYHEM_DIST"
		runtime "Release"
		optimize "on"