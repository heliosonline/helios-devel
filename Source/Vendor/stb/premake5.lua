-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Vendor.stb"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"
	kind          "StaticLib"
	targetname    "stb"

	targetdir (dir_bin   .. dir_group .. dir_config .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)

	includedirs {
	}


	files {
		"**.h",
		"**.cpp"
	}
