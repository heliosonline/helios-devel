project "Premake"
	kind "Utility"

    targetdir (dir_bin   .. dir_group .. dir_config .. dir_project)
    objdir    (dir_build .. dir_group .. dir_config .. dir_project)


	files {
		"%{wks.location}/premake5.lua",
		"%{wks.location}/**.lua"
	}


	postbuildmessage "Regenerating project files with Premake5..."
	postbuildcommands {
		"\"%{prj.location}/" .. os.host() .. "/premake5\" %{_ACTION} --file=\"%{wks.location}premake5.lua\""
	}
