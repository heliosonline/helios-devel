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
		"%{wks.location}/source/client/Engine/source"
	}
	links { "Engine" }
end
