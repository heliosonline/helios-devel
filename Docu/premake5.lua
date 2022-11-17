project "Documentation"
	kind "None"

	targetdir ("%{wks.location}/_bin/%{prj.name}")
	objdir ("%{wks.location}/_build/%{prj.name}")

	files {
		"%{wks.location}/Docu/*premake5.lua",
		"%{wks.location}/**.md"
	}
