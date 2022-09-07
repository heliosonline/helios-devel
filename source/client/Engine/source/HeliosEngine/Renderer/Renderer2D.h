#pragma once

#include "HeliosEngine/Renderer/OrthographicCamera.h"


namespace Helios {


	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

//		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

//		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

//		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
//		struct SceneData
//		{
//			glm::mat4 ViewProjectionMatrix;
//		};

//		static Scope<SceneData> s_SceneData;
	};


} // namespace Helios
