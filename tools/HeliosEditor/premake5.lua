-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "HeliosEditor"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	
	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)
	
	pchheader "pch.h"
	pchsource "source/pch.cpp"

	-- Libraries
	LibCommon{}
--	LibNet{}
	LibHeliosEngine{}


	includedirs {
		"source",
	}
	
	
	links {
		"Engine"
	}


	dependson {
		"build_inc"
	}


	files {
		"source/**.h",
		"source/**.cpp"
	}


	filter "configurations:Debug"
	
		kind "ConsoleApp"

		defines {
		}
	

	filter "configurations:Release"
		
		kind "WindowedApp"

		defines {
		}


	filter {}
		

	prebuildmessage "Updating version information..."
	prebuildcommands {
		"\"%{wks.location}tools/build_inc/bin/build_inc_" .. os.host() .. "\" -bfile \"%{prj.location}source/config/version.h\" -bdef VERSION_BUILD"
	}
	

	postbuildmessage "Copying assets to the target folder..."
	postbuildcommands {
		"{COPYDIR} %{wks.location}source/client/Engine/Assets " .. dir_bin .. dir_group .. dir_config .. "Assets",
		"{COPYDIR} %{prj.location}Assets " .. dir_bin .. dir_group .. dir_config .. "Assets"
	}
