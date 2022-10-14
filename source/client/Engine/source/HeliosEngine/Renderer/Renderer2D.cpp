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


	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
	};


	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;
	};


	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
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
		glm::vec4 QuadVertexPositions[4] =
			{ glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(0) };

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Text
		Ref<VertexArray> TextVertexArray;
		Ref<VertexBuffer> TextVertexBuffer;
		Ref<Shader> TextShader;
		glm::vec4 TextVertexPositions[4] =
			{ glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(0) };

		uint32_t TextIndexCount = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr = nullptr;

		// Circles
		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		// Lines
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 1.0f;

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

		// Quads
		{
			s_Data.QuadVertexArray = VertexArray::Create();
			s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
			s_Data.QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     },
				{ ShaderDataType::Float,  "a_TilingFactor" }
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
			Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
			s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
			delete[] quadIndices;

			s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

			s_Data.QuadShader = Shader::Create("Assets/Shaders/Renderer2D_Quad.glsl");
		} // Quads

		// Text
		{
			s_Data.TextVertexArray = VertexArray::Create();
			s_Data.TextVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex));
			s_Data.TextVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     }
				});
			s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
			s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];

			uint32_t* textIndices = new uint32_t[s_Data.MaxIndices];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
			{
				textIndices[i + 0] = offset + 0;
				textIndices[i + 1] = offset + 1;
				textIndices[i + 2] = offset + 2;

				textIndices[i + 3] = offset + 2;
				textIndices[i + 4] = offset + 3;
				textIndices[i + 5] = offset + 0;

				offset += 4;
			}
			Ref<IndexBuffer> textIB = IndexBuffer::Create(textIndices, s_Data.MaxIndices);
			s_Data.TextVertexArray->SetIndexBuffer(textIB);
			delete[] textIndices;

			s_Data.TextVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.TextVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
			s_Data.TextVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
			s_Data.TextVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

			s_Data.TextShader = Shader::Create("Assets/Shaders/Renderer2D_Text.glsl");
		} // Text

		// Circles
		{
			s_Data.CircleVertexArray = VertexArray::Create();
			s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
			s_Data.CircleVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_WorldPosition" },
				{ ShaderDataType::Float3, "a_LocalPosition" },
				{ ShaderDataType::Float4, "a_Color"         },
				{ ShaderDataType::Float,  "a_Thickness"     },
				{ ShaderDataType::Float,  "a_Fade"          }
			});
			s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
			s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

			uint32_t* circleIndices = new uint32_t[s_Data.MaxIndices];
			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
			{
				circleIndices[i + 0] = offset + 0;
				circleIndices[i + 1] = offset + 1;
				circleIndices[i + 2] = offset + 2;

				circleIndices[i + 3] = offset + 2;
				circleIndices[i + 4] = offset + 3;
				circleIndices[i + 5] = offset + 0;

				offset += 4;
			}
			Ref<IndexBuffer> circleIB = IndexBuffer::Create(circleIndices, s_Data.MaxIndices);
			s_Data.CircleVertexArray->SetIndexBuffer(circleIB);
			delete[] circleIndices;

			s_Data.CircleShader = Shader::Create("Assets/Shaders/Renderer2D_Circle.glsl");
		} // Circles

		// Lines
		{
			s_Data.LineVertexArray = VertexArray::Create();
			s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
			s_Data.LineVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color"    }
			});
			s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
			s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

			s_Data.LineShader = Shader::Create("Assets/Shaders/Renderer2D_Line.glsl");
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
		delete[] s_Data.TextVertexBufferBase;
		delete[] s_Data.LineVertexBufferBase;
	}


	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HE_PROFILER_FUNCTION();

		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.TextShader->Bind();
		s_Data.TextShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

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

		s_Data.TextIndexCount = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

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

		// Lines
		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		} // Lines

		// Circles
		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		} // Circles

		// Quads
		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

			s_Data.Stats.DrawCalls++;
		} // Quads

		// Text
		if (s_Data.TextIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
			s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
				s_Data.TextureSlots[i]->Bind(i);

			s_Data.TextShader->Bind();
			RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextIndexCount);

			s_Data.Stats.DrawCalls++;
		} // Text
	}


	// colored quad with 2D position
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, s_Data.WhiteTexture, 1.0f, color); }
	// colored quad with 3D position
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
		{ DrawQuad(position, size, s_Data.WhiteTexture, 1.0f, color); }


	// textured quad with 2D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling, glm::vec4(1.0f)); }
	// textured quad with 2D position and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, texture, 1.0f, color); }
	// textured quad with 2D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, texture, tiling, color); }
	// textured quad with 3D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling)
		{ DrawQuad(position, size, texture, tiling, glm::vec4(1.0f)); }
	// textured quad with 3D position and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color)
		{ DrawQuad(position, size, texture, 1.0f, color); }
	// textured quad with 3D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		DrawQuad(transform, texture, texCoords, tiling, color);
	}


	// textured quad with 2D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tiling)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tiling, glm::vec4(1.0f)); 	}
	// textured quad with 2D position and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, 1.0f, color); }
	// textured quad with 2D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tiling, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, subtexture, tiling, color); }
	// textured quad with 3D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tiling)
		{ DrawQuad(position, size, subtexture, tiling, glm::vec4(1.0f)); }
	// textured quad with 3D position and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, const glm::vec4& color)
		{ DrawQuad(position, size, subtexture, 1.0f, color); }
	// textured quad with 3D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subtexture, float tiling, const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture->GetTexture(), subtexture->GetTexCoords(), tiling, color);
	}


	// colored rotated quad with 2D position
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, rotation, s_Data.WhiteTexture, 1.0f, color); }
	// colored rotated quad with 3D position
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
		{ DrawQuad(position, size, rotation, s_Data.WhiteTexture, 1.0f, color); }


	// textured rotated quad with 2D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling, glm::vec4(1.0f)); }
	// textured rotated quad with 2D position and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, 1.0f, color); }
	// textured rotated quad with 2D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tiling, color); }
	// textured rotated quad with 3D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling)
		{ DrawQuad(position, size, rotation, texture, tiling, glm::vec4(1.0f)); }
	// textured rotated quad with 3D position and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color)
		{ DrawQuad(position, size, rotation, texture, 1.0f, color); }
	// textured rotated quad with 3D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		constexpr glm::vec2 texCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		DrawQuad(transform, texture, texCoords, tiling, color);
	}


	// textured rotated quad with 2D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tiling)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tiling, glm::vec4(1.0f)); }
	// textured rotated quad with 2D position and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, 1.0f, color); }
	// textured rotated quad with 2D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tiling, const glm::vec4& color)
		{ DrawQuad({ position.x, position.y, 0.0f }, size, rotation, subtexture, tiling, color); }
	// textured rotated quad with 3D position (optional tiling)
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tiling)
		{ DrawQuad(position, size, rotation, subtexture, tiling, glm::vec4(1.0f)); }
	// textured rotated quad with 3D position and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, const glm::vec4& color)
		{ DrawQuad(position, size, rotation, subtexture, 1.0f, color); }
	// textured rotated quad with 3D position, tiling and color
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subtexture, float tiling, const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, subtexture->GetTexture(), subtexture->GetTexCoords(), tiling, color);
	}


	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec2* texCoords, float tiling, const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		constexpr size_t quadVertexCount = 4;

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
			s_Data.QuadVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tiling;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}


	// Line with 2D position
	void Renderer2D::DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color)
	{
		DrawLine({ p0.x, p0.y, 0.0f }, { p1.x, p1.y, 0.0f }, color);
	}


	// Line with 3D position
	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;

		s_Data.Stats.LineCount++;
	}


	// Rectangle with 2D position
	void Renderer2D::DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRect({ position.x, position.y, 0.0f }, size, color);
	}


	// Rectangle with 3D position
	void Renderer2D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::vec3 p0 = glm::vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p1 = glm::vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		glm::vec3 p2 = glm::vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		glm::vec3 p3 = glm::vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}


	// Circle with 2D position
	void Renderer2D::DrawCircle(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float thickness, float fade)
	{
		DrawCircle({ position.x, position.y, 0.0f }, size, color, thickness, fade);
	}
	// Circle with 3D position
	void Renderer2D::DrawCircle(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, float thickness, float fade)
	{
		HE_PROFILER_FUNCTION();

		glm::mat4 transform =
			  glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawCircle(transform, color, thickness, fade);
	}
	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
	{
		HE_PROFILER_FUNCTION();

		if (s_Data.CircleIndexCount >= Renderer2DData::MaxIndices)
		 	NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPositions[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPositions[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr++;
		}
		s_Data.CircleIndexCount += 6;

		s_Data.Stats.CircleCount++;
	}


	float Renderer2D::SetLineWidth(float width)
	{
		float oldWidth = s_Data.LineWidth;
		s_Data.LineWidth = width;
		return oldWidth;
	}


	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}


	// String with 2D position
	void Renderer2D::DrawString(Ref<Font>& font, const std::string& text, const glm::vec2& position, float size, const glm::vec4& color)
		{ DrawString(font, text, { position.x, position.y, 0.0f }, size, 0.0f, color); }
	// String with 3D position
	void Renderer2D::DrawString(Ref<Font>& font, const std::string& text, const glm::vec3& position, float size, const glm::vec4& color)
		{ DrawString(font, text, position, size, 0.0f, color); }
	// Rotated string with 2D position
	void Renderer2D::DrawString(Ref<Font>& font, const std::string& text, const glm::vec2& position, float size, float rotation, const glm::vec4& color)
		{ DrawString(font, text, { position.x, position.y, 0.0f }, size, rotation, color); }
	// Rotated string with 3D position
	void Renderer2D::DrawString(Ref<Font>& font, const std::string& text, const glm::vec3& position, float size, float rotation, const glm::vec4& color)
	{
		HE_PROFILER_FUNCTION();

		auto fm = font->GetFontMetrics();
		glm::vec3 pos = position;
		float scale = 1.0f / fm.LineHeight;
		pos.y += (size * fm.Descender * scale);

		char32_t c_prev = 0;
		float offset_x = 0.0f;
		float offset_y = 0.0f;
		for (char32_t c_next : text)
		{
			switch (c_next)
			{
			case '\n':
				{
					c_prev = 0;
					offset_x = 0.0f;
					continue;
				}
			case '\r':
				{
					offset_y -= fm.LineHeight;
					continue;
				}
			case '\t':
				{
					c_prev = ' ';
					float tab = 4 * font->GetAdvance(' ');
					offset_x = ((int)((offset_x + 0.25f * tab) / tab)) + tab;
					continue;
				}
			default:
				{
					auto m = font->GetGlyphMetrics((char32_t)c_next);
					glm::mat4 transform =
						glm::translate(glm::mat4(1.0f), pos)
						* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
						* glm::scale(glm::mat4(1.0f), { size * scale, size * scale, 1.0f });
					glm::vec2 tex_coords[4] = {
						{ m.AtlasCoords.l, m.AtlasCoords.b },
						{ m.AtlasCoords.r, m.AtlasCoords.b },
						{ m.AtlasCoords.r, m.AtlasCoords.t },
						{ m.AtlasCoords.l, m.AtlasCoords.t }
					};
					glm::vec4 vertices[4] = {
						{ m.BaselineCoords.l + offset_x, m.BaselineCoords.b + offset_y, 0.0f, 1.0f},
						{ m.BaselineCoords.r + offset_x, m.BaselineCoords.b + offset_y, 0.0f, 1.0f },
						{ m.BaselineCoords.r + offset_x, m.BaselineCoords.t + offset_y, 0.0f, 1.0f },
						{ m.BaselineCoords.l + offset_x, m.BaselineCoords.t + offset_y, 0.0f, 1.0f }
					};
					DrawChar(transform, vertices, font->GetAtlasTexture(), tex_coords, color);
					offset_x += font->GetAdvance(c_prev, c_next);
					c_prev = c_next;
				}
			} // switch
		} // for
	}


	void Renderer2D::DrawChar(const glm::mat4& transform, const glm::vec4* vertices, const Ref<Texture2D>& texture, const glm::vec2* texCoords, const glm::vec4& color)
	{
		constexpr size_t textVertexCount = 4;

		if (s_Data.TextIndexCount >= Renderer2DData::MaxIndices)
			NextBatch();

		float textureIndex = 0.0f; // White Texture
		if (*s_Data.TextureSlots[0] != *texture)
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

		for (size_t i = 0; i < textVertexCount; i++)
		{
			s_Data.TextVertexBufferPtr->Position = transform * vertices[i];
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoords[i];
			s_Data.TextVertexBufferPtr->TexIndex = textureIndex;
			s_Data.TextVertexBufferPtr++;
		}

		s_Data.TextIndexCount += 6;

		s_Data.Stats.GlyphCount++;
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
