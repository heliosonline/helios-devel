#include "pch.h"

#include "HeliosEngine/Renderer/RendererAPI.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLRendererAPI.h"
#endif


namespace Helios {


	RendererAPI::API RendererAPI::s_API = DEFAULT_RENDERERAPI;


	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateScope<GLRendererAPI>();
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
