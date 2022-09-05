project "Documentation"
	kind "None"
	location ("%{wks.location}")

	targetdir ("%{wks.location}/bin/%{prj.name}")
	objdir ("%{wks.location}/build/%{prj.name}")

	files {
		"%{wks.location}/docu/*premake5.lua",
		"%{wks.location}/**.md"
	}
