-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Client"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "ConsoleApp"
	
	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)
	
	pchheader "pch.h"
	pchsource "source/pch.cpp"

	-- Libraries
	LibCommon{}
	LibNet{}
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
	
		defines {
		}
	

	filter "configurations:Release"
		
		defines {
		}


	filter {}
		

	prebuildmessage "Updating version information..."
	prebuildcommands {
		"\"%{wks.location}tools/build_inc/bin/build_inc_" .. os.host() .. "\" -bfile \"%{prj.location}source/config/version.h\" -bdef VERSION_BUILD"
	}
