#include "pch_engine.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/VertexArray.h"

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLVertexArray.h"
#endif


namespace Helios {


	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:  return CreateRef<GLVertexArray>();
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
