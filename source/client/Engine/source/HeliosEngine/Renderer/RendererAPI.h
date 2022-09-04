#pragma once

#include "HeliosEngine/Renderer/VertexArray.h"

#include <glm/glm.hpp>


namespace Helios {


// related on build options and platform
#if   defined HE_BUILDWITH_RENDERER_DIRECTX
#		define HE_DEFAULT_RENDERERAPI RendererAPI::API::DirectX
#elif defined HE_BUILDWITH_RENDERER_VULKAN
#		define HE_DEFAULT_RENDERERAPI RendererAPI::API::Vulkan
#elif defined HE_BUILDWITH_RENDERER_OPENGL
#		define HE_DEFAULT_RENDERERAPI RendererAPI::API::OpenGL
#endif


	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,

// related on build options and platform
#ifdef HE_BUILDWITH_RENDERER_DIRECTX
			DirectX,
#endif
#ifdef HE_BUILDWITH_RENDERER_VULKAN
			Vulkan,
#endif
#ifdef HE_BUILDWITH_RENDERER_OPENGL
			OpenGL,
#endif

		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};


} // namespace Helios
