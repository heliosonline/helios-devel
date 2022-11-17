-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "build_inc"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "ConsoleApp"
	
    targetdir ("_bin")
    objdir    (dir_build .. dir_group .. dir_config .. dir_project)
    targetsuffix ("_" .. os.host())


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
