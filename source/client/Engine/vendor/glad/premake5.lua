-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Engine.glad2"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "StaticLib"
	targetname    "glad2"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. "engine.vendor/" .. dir_project)
	
	includedirs {
		"include",
		"src"
	}
	
	
	files {
		"include/**.h",
		"src/*.c"
	}
