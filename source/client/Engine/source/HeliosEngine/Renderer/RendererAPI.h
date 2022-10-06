#pragma once

#include "HeliosEngine/Renderer/VertexArray.h"

#include <glm/glm.hpp>


namespace Helios {


// related on build options and platform
#if   defined BUILDWITH_RENDERER_DIRECTX
#		define DEFAULT_RENDERERAPI RendererAPI::API::DirectX
#elif defined BUILDWITH_RENDERER_METAL
#		define DEFAULT_RENDERERAPI RendererAPI::API::Metal
#elif defined BUILDWITH_RENDERER_VULKAN
#		define DEFAULT_RENDERERAPI RendererAPI::API::Vulkan
#elif defined BUILDWITH_RENDERER_OPENGL
#		define DEFAULT_RENDERERAPI RendererAPI::API::OpenGL
#endif


	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			DirectX,
#endif
#ifdef BUILDWITH_RENDERER_METAL
			Metal,
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			Vulkan,
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			OpenGL,
#endif

		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		virtual void SetLineWidth(float width) = 0;

		inline static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};


} // namespace Helios
