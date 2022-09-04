#include "pch_engine.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Buffer.h"

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLBuffer.h"
#endif


namespace Helios {


	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateRef<GLVertexBuffer>(vertices, size);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is not supported!"); return nullptr;

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL: return CreateRef<GLIndexBuffer>(indices, count);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
