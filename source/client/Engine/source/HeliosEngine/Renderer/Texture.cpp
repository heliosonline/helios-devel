#include "pch_engine.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Texture.h"

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLTexture.h"
#endif


namespace Helios {


	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_OPENGL
		case RendererAPI::API::OpenGL:  return CreateRef<GLTexture2D>(width, height);
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
#ifdef HE_BUILDWITH_RENDERER_OPENGL
		case RendererAPI::API::OpenGL:  return CreateRef<GLTexture2D>(path);
#endif

		default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


} // namespace Helios
