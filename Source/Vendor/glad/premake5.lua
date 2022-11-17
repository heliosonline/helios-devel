-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Vendor.glad"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "StaticLib"
	targetname    "glad"

	targetdir (dir_bin   .. dir_group .. dir_config .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)

	includedirs {
		"include",
		"src"
	}


	files {
		"include/**.h",
		"src/*.c"
	}
