-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Vendor.imgui"
	kind          "StaticLib"
	architecture  "x86_64"
	language      "C"
	cppdialect    "C++20"
	staticruntime "On"
	targetname    "imgui"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)

	-- move project in the correct dir
	basedir("imgui")


	files {
		"imgui/*.h",
		"imgui/*.cpp",
	}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
