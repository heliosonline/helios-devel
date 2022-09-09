#include "pch.h"

#include "HeliosEngine/Renderer/Renderer2D.h"

#include "HeliosEngine/Renderer/VertexArray.h"
#include "HeliosEngine/Renderer/Shader.h"
#include "HeliosEngine/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Helios {


	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};
	static Renderer2DStorage* s_Data;


	void Renderer2D::Init()
	{
		HE_PROFILE_FUNCTION();

		s_Data = new Renderer2DStorage();

		s_Data->QuadVertexArray = VertexArray::Create();
		float vertices[(3 + 2) * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left
		};
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};
		vb->SetLayout(layout);
		s_Data->QuadVertexArray->AddVertexBuffer(vb);
		unsigned int indices[6] = {
			0, 1, 2, 2, 3, 0
		};
		Ref<IndexBuffer>ib = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(ib);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);
	}


	void Renderer2D::Shutdown()
	{
		HE_PROFILE_FUNCTION();

		delete s_Data;
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HE_PROFILE_FUNCTION();

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}


	void Renderer2D::EndScene()
	{
		HE_PROFILE_FUNCTION();

	}


	// colored quad with 2D position
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, s_Data->WhiteTexture, 1.0f, color);
	}


	// colored quad with 3D position
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, s_Data->WhiteTexture, 1.0f, color);
	}


	// textured quad with 2D position
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling, glm::vec4(1.0f));
	}


	// textured quad with 2D position
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, 1.0f, color);
	}


	// textured quad with 2D position
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling, color);
	}



	// textured quad with 3D position
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling)
	{
		DrawQuad(position, size, texture, tiling, glm::vec4(1.0f));
	}


	// textured quad with 3D position
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawQuad(position, size, texture, 1.0f, color);
	}


	// textured quad with 3D position
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		HE_PROFILE_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tiling, color);
	}


	// colored rotated quad with 2D position
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, s_Data->WhiteTexture, 1.0f, color);
	}


	// colored rotated quad with 3D position
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(position, size, rotation, s_Data->WhiteTexture, 1.0f, color);
	}


	// textured rotated quad with 2D position
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling, glm::vec4(1.0f));
	}


	// textured rotated quad with 2D position
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, 1.0f, color);
	}


	// textured rotated quad with 2D position
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling, color);
	}


	// textured rotated quad with 3D position
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling)
	{
		DrawRotatedQuad(position, size, rotation, texture, tiling, glm::vec4(1.0f));
	}


	// textured rotated quad with 3D position
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color)
	{
		DrawRotatedQuad(position, size, rotation, texture, 1.0f, color);
	}


	// textured rotated quad with 3D position
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		HE_PROFILE_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tiling, color);
	}


	void Renderer2D::DrawQuad(const glm::mat4 transform, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		texture->Bind();

		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->TextureShader->SetFloat4("u_Color", color);
		s_Data->TextureShader->SetFloat("u_TilingFactor", tiling);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}


} // namespace Helios
