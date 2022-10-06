#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Framebuffer.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
#	include "Platform/Renderer/DirectX/DXFramebuffer.h"
#endif
#ifdef BUILDWITH_RENDERER_METAL
#	include "Platform/Renderer/Metal/MTFramebuffer.h"
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
#	include "Platform/Renderer/Vulkan/VKFramebuffer.h"
#endif
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
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX: return CreateRef<DXFramebuffer>(spec);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal: return CreateRef<MTFramebuffer>(spec);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan: return CreateRef<VKFramebuffer>(spec);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateRef<GLFramebuffer>(spec);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}

} // namespace Helios
