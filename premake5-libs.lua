function LibCommon()
	includedirs {
		"%{wks.location}/source/common/LibCommon/source"
	}
	links { "LibCommon" }
end


function LibNet()
	includedirs {
		"%{wks.location}/source/common/LibNet/source"
	}
	links { "LibNet" }
	LibASIO {}
end


function LibASIO()
	includedirs {
		"%{wks.location}/source/vendor/asio-1.24.0/include"
	}
end


function LibHeliosEngine()
	includedirs {
		"%{wks.location}/source/client/Engine/source",
		"%{wks.location}/source/client/Engine/vendor/spdlog/include",
--		"%{wks.location}/source/client/Engine/vendor/glad/include",
		"%{wks.location}/source/client/Engine/vendor/glfw/include",
		"%{wks.location}/source/client/Engine/vendor/glm",
		"%{wks.location}/source/client/Engine/vendor/imgui",
--		"%{wks.location}/source/client/Engine/vendor/stb",
--		"%{wks.location}/source/client/Engine/vendor/lua/lua-5.4.4",
--		"%{wks.location}/source/client/Engine/vendor/lua/extra",
	}
	defines {
		"GLFW_INCLUDE_NONE"
	}
	links { "Engine" }
end
