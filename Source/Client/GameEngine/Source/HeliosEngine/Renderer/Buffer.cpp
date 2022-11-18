#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Buffer.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
#	include "Platform/Renderer/DirectX/DXBuffer.h"
#endif
#ifdef BUILDWITH_RENDERER_METAL
#	include "Platform/Renderer/Metal/MTBuffer.h"
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
#	include "Platform/Renderer/Vulkan/VKBuffer.h"
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLBuffer.h"
#endif


namespace Helios {


	//////////////////////////////////////////////////////////////////////////////
	// VertexBuffer //////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////


	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX: return CreateRef<DXVertexBuffer>(size);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal: return CreateRef<MTVertexBuffer>(size);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan: return CreateRef<VKVertexBuffer>(size);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateRef<GLVertexBuffer>(size);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX: return CreateRef<DXVertexBuffer>(vertices, size);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal: return CreateRef<MTVertexBuffer>(vertices, size);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan: return CreateRef<VKVertexBuffer>(vertices, size);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateRef<GLVertexBuffer>(vertices, size);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


	//////////////////////////////////////////////////////////////////////////////
	// IndexBuffer ///////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX: return CreateRef<DXIndexBuffer>(indices, count);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal: return CreateRef<MTIndexBuffer>(indices, count);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan: return CreateRef<VKIndexBuffer>(indices, count);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateRef<GLIndexBuffer>(indices, count);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
