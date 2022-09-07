#include "pch.h"

#include "HeliosEngine/Renderer/Renderer2D.h"

#include "HeliosEngine/Renderer/VertexArray.h"
#include "HeliosEngine/Renderer/Shader.h"
#include "HeliosEngine/Renderer/RenderCommand.h"


namespace Helios {


	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};
	static Renderer2DStorage* s_Data;


	void Renderer2D::Init()
	{
		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();
		float vertices[(3 + 0) * 4] = {
			-0.5f, -0.5f, 0.0f, // bottom left
			 0.5f, -0.5f, 0.0f, // bottom right
			 0.5f,  0.5f, 0.0f, // top right
			-0.5f,  0.5f, 0.0f  // top left
		};
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
		};
		vb->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(vb);
		unsigned int indices[6] = {
			0, 1, 2, 2, 3, 0
		};
		Ref<IndexBuffer>ib = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(ib);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColorShader.glsl");
	}


	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetProjectionMatrix());
		s_Data->FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0f));
	}


	void Renderer2D::EndScene()
	{

	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}


	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}


} // namespace Helios
