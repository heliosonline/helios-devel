#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/GraphicsContext.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLContext.h"
#endif


namespace Helios {


	Scope<GraphicsContext> GraphicsContext::Create(GLFWwindow* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateScope<GLContext>(window);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
