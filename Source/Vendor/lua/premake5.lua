-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "Vendor.lua"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++17"
	staticruntime "On"
	kind          "StaticLib"
	targetname    "lua"
	
	targetdir (dir_bin   .. dir_group .. dir_config .. dir_project)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)
	
	includedirs {
		"lua-5.4.4/lua"
	}
	
	
	files {
		"lua-5.4.4/lua/**.h",
		"lua-5.4.4/lua/lua_one_file.c"
	}
