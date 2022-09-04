---------------------------------
-- [ WORKSPACE CONFIGURATION ] --
---------------------------------
workspace "Helios-Development"

	configurations {
		"Debug",
		"Release"
	}

	platforms {
		"Windows",
		"Linux",
		"MacOS"
	}

	--------------------------------
	-- [ PLATFORM CONFIGURATION ] --
	--------------------------------
	filter "platforms:Windows"
		system  "windows"
		defines "_SYSTEM_WINDOWS"

	filter "platforms:Linux"
		system  "linux"
		defines "_SYSTEM_LINUX"

	filter "platforms:MacOS"
		system  "macosx"
		defines "_SYSTEM_MACOS"

	-- Windows only
	filter { "system:windows", "action:vs*" }
		flags { "MultiProcessorCompile", "NoMinimalRebuild" }
		systemversion "latest"

	-------------------------------------
	-- [ DEBUG/RELEASE CONFIGURATION ] --
	-------------------------------------
	filter "configurations:Debug"
		defines  "_BUILD_DEBUG"
		symbols  "On"
		optimize "Off"
		runtime  "Debug"

	filter "configurations:Release"
		defines  "_BUILD_RELEASE"
		symbols  "Off"
		optimize "Speed"
		runtime  "Release"

	filter {}

	-------------------------------
	-- [ LIBRARIES (UTIL-FUNC) ] --
	-------------------------------
	
	include("premake5-libs.lua")

	-------------------------------
	-- [ PROJECT CONFIGURATION ] --
	-------------------------------

	dir_bin     = "%{wks.location}/bin/"
	dir_build   = "%{wks.location}/build/"
	dir_config  = "%{string.lower(cfg.platform)}-%{string.lower(cfg.buildcfg)}/"
	dir_project = "%{string.lower(prj.name)}"

	group "client"
		dir_group = "client/"
		include("source/client/")

	group "server"
		dir_group = "server/"
		include("source/server/")

	group "common"
		dir_group = "common/"
		include("source/common/")

	group "vendor"
		dir_group = "vendor/"
		include("vendor/")

	group "tools"
		dir_group = "tools/"
		include("tools/build_inc/")

	group ""
