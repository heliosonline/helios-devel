#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Texture.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
#	include "Platform/Renderer/DirectX/DXTexture.h"
#endif
#ifdef BUILDWITH_RENDERER_METAL
#	include "Platform/Renderer/Metal/MTTexture.h"
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
#	include "Platform/Renderer/Vulkan/VKTexture.h"
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLTexture.h"
#endif


namespace Helios {


	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, int bpp)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX:  return CreateRef<DXTexture2D>(width, height, bpp);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal:  return CreateRef<MTTexture2D>(width, height, bpp);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan:  return CreateRef<VKTexture2D>(width, height, bpp);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:  return CreateRef<GLTexture2D>(width, height, bpp);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX:  return CreateRef<DXTexture2D>(path);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal:  return CreateRef<MTTexture2D>(path);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan:  return CreateRef<VKTexture2D>(path);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:  return CreateRef<GLTexture2D>(path);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
