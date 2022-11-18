-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "GameEngine"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "StaticLib"

	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)

	pchheader "pch.h"
	pchsource "Source/pch.cpp"

	-- Libraries
	LibCommon{}
	LibLuaHelper{}
	VendorGlad{}
	VendorGlfw{}
	VendorGlm{}
	VendorImgui{}
	VendorJson{}
	VendorLua{}
	VendorMsdfAtlas{}
	VendorSpdlog{}
	VendorStb{}


	includedirs {
		"source",
	}


	links {
	}


	dependson {
	}


	files {
		"Source/pch.h",
		"Source/pch.cpp",
		"Source/HeliosEngine/**.h",
		"Source/HeliosEngine/**.cpp",
		"Source/Platform/PlatformDetection.h",

		"Assets/**.glsl",
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
--			"BUILDWITH_RENDERER_METAL",
			"BUILDWITH_RENDERER_OPENGL",
--			"BUILDWITH_RENDERER_VULKAN",
		}

		files {
			"Source/Platform/System/Windows/**.h",
			"Source/Platform/System/Windows/**.cpp",
--			"Source/Platform/Renderer/DirectX/**.h",
--			"Source/Platform/Renderer/DirectX/**.cpp",
--			"Source/Platform/Renderer/Metal/**.h",
--			"Source/Platform/Renderer/Metal/**.cpp",
			"Source/Platform/Renderer/OpenGL/**.h",
			"Source/Platform/Renderer/OpenGL/**.cpp",
--			"Source/Platform/Renderer/Vulkan/**.h",
--			"Source/Platform/Renderer/Vulkan/**.cpp",
		}


	filter "platforms:Linux"

		defines {
--			"BUILDWITH_RENDERER_DIRECTX",
--			"BUILDWITH_RENDERER_METAL",
			"BUILDWITH_RENDERER_OPENGL",
--			"BUILDWITH_RENDERER_VULKAN",
		}

		files {
			"Source/Platform/System/Linux/**.h",
			"Source/Platform/System/Linux/**.cpp",
--			"Source/Platform/Renderer/DirectX/**.h",
--			"Source/Platform/Renderer/DirectX/**.cpp",
--			"Source/Platform/Renderer/Metal/**.h",
--			"Source/Platform/Renderer/Metal/**.cpp",
			"Source/Platform/Renderer/OpenGL/**.h",
			"Source/Platform/Renderer/OpenGL/**.cpp",
--			"Source/Platform/Renderer/Vulkan/**.h",
--			"Source/Platform/Renderer/Vulkan/**.cpp",
		}


	filter "platforms:MacOS"

		defines {
--			"BUILDWITH_RENDERER_DIRECTX",
--			"BUILDWITH_RENDERER_METAL",
			"BUILDWITH_RENDERER_OPENGL",
--			"BUILDWITH_RENDERER_VULKAN",
		}

		files {
			"Source/Platform/System/MacOS/**.h",
			"Source/Platform/System/MacOS/**.cpp",
--			"Source/Platform/Renderer/DirectX/**.h",
--			"Source/Platform/Renderer/DirectX/**.cpp",
--			"Source/Platform/Renderer/Metal/**.h",
--			"Source/Platform/Renderer/Metal/**.cpp",
			"Source/Platform/Renderer/OpenGL/**.h",
			"Source/Platform/Renderer/OpenGL/**.cpp",
--			"Source/Platform/Renderer/Vulkan/**.h",
--			"Source/Platform/Renderer/Vulkan/**.cpp",
		}


	filter {}


	postbuildmessage "Copying assets to the target folder..."
	postbuildcommands {
		"{COPYDIR} %{wks.location}Source/Client/GameEngine/Assets " .. dir_bin .. dir_group .. dir_config .. "Assets"
	}
