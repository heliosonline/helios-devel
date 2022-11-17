-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "ConnHandler"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "ConsoleApp"
	
	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)
	
	pchheader "pch.h"
	pchsource "Source/pch.cpp"

	-- Libraries
	LibCommon{}
	LibNet{}


	includedirs {
		"Source",
	}
	
	
	links {
	}


	dependson {
		"build_inc"
	}


	files {
		"Source/**.h",
		"Source/**.cpp"
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
		"\"%{wks.location}Tools/build_inc/_bin/build_inc_" .. os.host() .. "\" -bfile \"%{prj.location}Source/Config/Version.h\" -bdef VERSION_BUILD"
	}
