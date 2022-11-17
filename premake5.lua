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

	filter "platforms:Linux"
		system  "linux"

	filter "platforms:MacOS"
		system  "macosx"

	-- Windows only
	filter { "system:windows", "action:vs*" }
		flags { "MultiProcessorCompile", "NoMinimalRebuild" }
		systemversion "latest"

	-------------------------------------
	-- [ DEBUG/RELEASE CONFIGURATION ] --
	-------------------------------------
	filter "configurations:Debug"
		defines {
			"BUILD_DEBUG",
			"_DEBUG",
		}
		symbols  "On"
		optimize "Off"
		runtime  "Debug"

	filter "configurations:Release"
		defines {
			"BUILD_RELEASE",
			"NDEBUG",
		}
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

	dir_bin     = "%{wks.location}/_bin/"
	dir_build   = "%{wks.location}/_build/"
	dir_config  = "%{string.lower(cfg.platform)}-%{string.lower(cfg.buildcfg)}/"
	dir_project = "%{string.lower(prj.name)}"

	group "Client"
		dir_group = "Client/"
		include("Source/Client/")

	group "Server"
		dir_group = "Server/"
		include("Source/Server/")

	group "Libs"
		dir_group = "Libs/"
		include("Source/Libs/")

	group "Vendor"
		dir_group = "Vendor/"
		include("Vendor/")
		include("Source/Vendor/")

	group "Misc"
		dir_group = "Misc/"
		include("Docu/")
		include("Tools/")

	group ""
