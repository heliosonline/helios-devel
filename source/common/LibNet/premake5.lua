-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "LibNet"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "StaticLib"
	
	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)
	
	-- Libraries
	LibCommon{}
	LibASIO{}


	includedirs {
		"source",
	}
	
	
	links {
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
