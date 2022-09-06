-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Engine"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "StaticLib"
	
	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)
	
	pchheader "pch.h"
	pchsource "source/pch.cpp"

	defines {
		"GLFW_INCLUDE_NONE"
	}

	-- Libraries
	LibCommon{}


	includedirs {
		"source",
		"vendor/spdlog/include",
		"vendor/glad/include",
		"vendor/glfw/include",
		"vendor/glm",
		"vendor/imgui",
		"vendor/stb",
	}
	
	
	links {
		"Engine.glad2",
		"Engine.glfw",
		"Engine.imgui",
	}


	dependson {
		""
	}


	files {
		"source/pch.h",
		"source/pch.cpp",
		"source/HeliosEngine/**.h",
		"source/HeliosEngine/**.cpp",
		"vendor/stb/*.h",
		"vendor/stb/*.cpp"
	}


	filter "configurations:Debug"
	
		defines {
			"LOG_LEVEL=1"
		}
	

	filter "configurations:Release"
		
		defines {
		}


	filter "platforms:Windows"

		defines {
--			"BUILDWITH_RENDERER_DIRECTX",
			"BUILDWITH_RENDERER_OPENGL",
--			"BUILDWITH_RENDERER_VULKAN",
		}

		files {
			"source/Platform/System/Windows/**.h",
			"source/Platform/System/Windows/**.cpp",
--			"source/Platform/Renderer/DirectX/**.h",
--			"source/Platform/Renderer/DirectX/**.cpp",
			"source/Platform/Renderer/OpenGL/**.h",
			"source/Platform/Renderer/OpenGL/**.cpp",
--			"source/Platform/Renderer/Vulkan/**.h",
--			"source/Platform/Renderer/Vulkan/**.cpp",
		}


	filter "platforms:Linux"

		defines {
--			"BUILDWITH_RENDERER_DIRECTX",
			"BUILDWITH_RENDERER_OPENGL",
--			"BUILDWITH_RENDERER_VULKAN",
		}

		files {
			"source/Platform/System/Linux/**.h",
			"source/Platform/System/Linux/**.cpp",
--			"source/Platform/Renderer/DirectX/**.h",
--			"source/Platform/Renderer/DirectX/**.cpp",
			"source/Platform/Renderer/OpenGL/**.h",
			"source/Platform/Renderer/OpenGL/**.cpp",
--			"source/Platform/Renderer/Vulkan/**.h",
--			"source/Platform/Renderer/Vulkan/**.cpp",
		}


	filter "platforms:MacOS"

		defines {
--			"BUILDWITH_RENDERER_DIRECTX",
			"BUILDWITH_RENDERER_OPENGL",
--			"BUILDWITH_RENDERER_VULKAN",
		}

		files {
			"source/Platform/System/MacOS/**.h",
			"source/Platform/System/MacOS/**.cpp",
--			"source/Platform/Renderer/DirectX/**.h",
--			"source/Platform/Renderer/DirectX/**.cpp",
			"source/Platform/Renderer/OpenGL/**.h",
			"source/Platform/Renderer/OpenGL/**.cpp",
--			"source/Platform/Renderer/Vulkan/**.h",
--			"source/Platform/Renderer/Vulkan/**.cpp",
		}


	filter {}
		

	group "client/Engine/vendor"
		include("vendor/glad")
		include("vendor/premake5-glfw.lua")
		include("vendor/premake5-imgui.lua")
