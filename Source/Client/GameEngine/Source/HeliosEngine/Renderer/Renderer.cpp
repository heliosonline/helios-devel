#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Renderer2D.h"


namespace Helios {


	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();


	void Renderer::Init()
	{
		HE_PROFILER_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}


	void Renderer::Shutdown()
	{
		HE_PROFILER_FUNCTION();

		Renderer2D::Shutdown();
	}


	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		HE_PROFILER_FUNCTION();

		RenderCommand::SetViewport(0, 0, width, height);
	}


	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		HE_PROFILER_FUNCTION();

		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}


	void Renderer::EndScene()
	{
		HE_PROFILER_FUNCTION();
	}


	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		HE_PROFILER_FUNCTION();

		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}


} // namespace Helios
