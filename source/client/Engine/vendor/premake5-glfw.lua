-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Engine.glfw"
	kind          "StaticLib"
	architecture  "x86_64"
	language      "C"
	cppdialect    "C++20"
	staticruntime "On"
	targetname    "glfw3"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)

	-- move project in the correct dir
	basedir("glfw")
	
	includedirs {
		"glfw/include",
		"glfw/src"
	}
	
	files {
		"glfw/include/GLFW/glfw3.h",
		"glfw/include/GLFW/glfw3native.h",
		"glfw/src/internal.h",
		"glfw/src/platform.h",
		"glfw/src/mappings.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/platform.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c",
		"glfw/src/egl_context.c",
		"glfw/src/osmesa_context.c",
		"glfw/src/null_platform.h",
		"glfw/src/null_joystick.h",
		"glfw/src/null_init.c",
		"glfw/src/null_monitor.c",
		"glfw/src/null_window.c",
		"glfw/src/null_joystick.c"
	}

--[[
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<

	filter "system:linux"
		pic "On"
		systemversion "latest"
		staticruntime "On"

		files {
			-- !APPLE && !WIN32
			"glfw/posix_time.h",
			"glfw/src/posix_thread.h",
			"glfw/src/posix_module.c",
			"glfw/src/posix_time.c",
			"glfw/src/posix_thread.c".
			-- GLFW_BUILD_X11
			"glfw/x11_platform.h",
			"glfw/src/xkb_unicode.h",
			"glfw/src/x11_init.c",
			"glfw/src/x11_monitor.c",
			"glfw/src/x11_window.c",
			"glfw/src/xkb_unicode.c",
			"glfw/src/glx_context.c",
			-- "Linux"
			"glfw/src/linux_joystick.c",
			-- GLFW_BUILD_X11 OR GLFW_BUILD_WAYLAND
			"glfw/posix_poll.h",
			"glfw/src/posix_poll.c"
		}

		defines {
			"_GLFW_X11",
			"_DEFAULT_SOURCE"
		}

	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
]]--

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files {
			-- WIN32
			"glfw/src/win32_time.h",
			"glfw/src/win32_thread.h",
			"glfw/src/win32_module.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			-- GLFW_BUILD_WIN32
			"glfw/src/win32_platform.h",
			"glfw/src/win32_joystick.h",
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c"
		}

		defines {
			"_GLFW_WIN32",
			"UNICODE", "_UNICODE",
			"_CRT_SECURE_NO_WARNINGS"
		}

--[[
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<

	filter "system:macosx"

		files {
			-- APPLE
			glfw/cocoa_time.h,
			glfw/cocoa_time.c,
			glfw/posix_thread.h,
			glfw/posix_module.c,
			glfw/posix_thread.c,
			-- GLFW_BUILD_COCOA
			glfw/cocoa_platform.h,
			glfw/cocoa_joystick.h,
			glfw/cocoa_init.m,
			glfw/cocoa_joystick.m,
			glfw/cocoa_monitor.m,
			glfw/cocoa_window.m,
			glfw/nsgl_context.m
		}

		defines {
			_GLFW_COCOA
		}

	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
	TODO: >>>>>> I CAN'T TEST THIS PART <<<<<<
]]--

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
