#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/VertexArray.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
#	include "Platform/Renderer/DirectX/DXVertexArray.h"
#endif
#ifdef BUILDWITH_RENDERER_METAL
#	include "Platform/Renderer/Metal/MTVertexArray.h"
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
#	include "Platform/Renderer/Vulkan/VKVertexArray.h"
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLVertexArray.h"
#endif


namespace Helios {


	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX:  return CreateRef<DXVertexArray>();
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal:  return CreateRef<MTVertexArray>();
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan:  return CreateRef<VKVertexArray>();
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:  return CreateRef<GLVertexArray>();
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
