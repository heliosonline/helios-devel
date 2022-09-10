#include "pch.h"

#include "HeliosEngine/Renderer/Renderer2D.h"

#include "HeliosEngine/Renderer/VertexArray.h"
#include "HeliosEngine/Renderer/Shader.h"
#include "HeliosEngine/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Helios {


	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};


	struct Renderer2DData
	{
		// Config (max...)
		static const uint32_t MaxQuads        = 20000;
		static const uint32_t MaxVertices     = MaxQuads * 4;
		static const uint32_t MaxIndices      = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		// Quads
		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		glm::vec4 QuadVertexPositions[4];

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Circles

		// Lines

		// Textures
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture
		Ref<Texture2D> WhiteTexture;

		// Stats
		Renderer2D::Statistics Stats;
	};
	static Renderer2DData s_Data;


	void Renderer2D::Init()
	{
		HE_PROFILER_FUNCTION();

		{ // Quads
			s_Data.QuadVertexArray = VertexArray::Create();

			s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
			s_Data.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float,  "a_TexIndex" },
				{ ShaderDataType::Float,  "a_TilingFactor" },
				});
			s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

			s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

			uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}
			Ref<IndexBuffer>quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
			s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
			delete[] quadIndices;

			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

			s_Data.QuadShader = Shader::Create("assets/shaders/Renderer2D_Quad.glsl");
		} // Quads

		{ // Circles
		} // Circles

		{ // Lines
		} // Lines

		{ // Textures
		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		} // Textures
	}


	void Renderer2D::Shutdown()
	{
		HE_PROFILER_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HE_PROFILER_FUNCTION();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		StartBatch();
	}


	void Renderer2D::EndScene()
	{
		HE_PROFILER_FUNCTION();

		Flush();
	}


	void Renderer2D::StartBatch()
	{
		HE_PROFILER_FUNCTION();

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}


	void Renderer2D::NextBatch()
	{
		HE_PROFILER_FUNCTION();

		Flush();
		StartBatch();
	}


	void Renderer2D::Flush()
	{
		HE_PROFILER_FUNCTION();

		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

			s_Data.Stats.DrawCalls++;
		}
	}


	// colored quad with 2D position
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, s_Data.WhiteTexture, 1.0f, color);
	}


	// colored quad with 3D position
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad(position, size, s_Data.WhiteTexture, 1.0f, color);
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
		HE_PROFILER_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tiling, color);
	}


	// colored rotated quad with 2D position
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, s_Data.WhiteTexture, 1.0f, color);
	}


	// colored rotated quad with 3D position
	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad(position, size, rotation, s_Data.WhiteTexture, 1.0f, color);
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
		HE_PROFILER_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, tiling, color);
	}


	void Renderer2D::DrawQuad(const glm::mat4 transform, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f; // White Texture
		if(*s_Data.TextureSlots[0] != *texture)
		{
			for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i] == *texture)
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
					NextBatch();

				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
				s_Data.TextureSlotIndex++;
			}
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tiling;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}


	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}


	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}


} // namespace Helios
