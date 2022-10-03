#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Framebuffer.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLFramebuffer.h"
#endif


namespace Helios {




	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;

			// related on build options and platform
#ifdef BUILDWITH_RENDERER_OPENGL
		case RendererAPI::API::OpenGL: return CreateRef<GLFramebuffer>(spec);
#endif

		default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

} // namespace Helios
