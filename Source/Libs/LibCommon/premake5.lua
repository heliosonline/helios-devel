-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "LibCommon"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "StaticLib"
	
	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)
	
--	pchheader "pch.h"
--	pchsource "Source/pch.cpp"


	includedirs {
		"Source",
	}
	
	
	links {
	}


	dependson {
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
