#pragma once

#include "HeliosEngine/Renderer/OrthographicCamera.h"
#include "HeliosEngine/Renderer/Texture.h"


namespace Helios {


	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		// Colored quad with 2D position
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		// Colored quad with 3D position
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		// Textured quad with 2D position (optional tiling)
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f);
		// Textured quad with 2D position and color
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);
		// Textured quad with 2D position, tiling and color
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color);
		// Textured quad with 3D position (optional tiling)
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f);
		// Textured quad with 3D position and color
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);
		// Textured quad with 3D position, tiling and color
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color);

		// Colored rotated quad with 2D position (rotation in degree)
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		// Colored rotated quad with 3D position (rotation in degree)
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		// Textured rotated quad with 2D position (optional tiling, rotation in degree)
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f);
		// Textured rotated quad with 2D position and color (rotation in degree)
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color);
		// Textured rotated quad with 2D position, tiling and color (rotation in degree)
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color);
		// Textured rotated quad with 3D position (optional tiling, rotation in degree)
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f);
		// Textured rotated quad with 3D position and color (rotation in degree)
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& color);
		// Textured rotated quad with 3D position, tiling and color (rotation in degree)
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color);

		// Intended to be used internally
		static void DrawQuad(const glm::mat4 transform, const Ref<Texture2D>& texture, float tiling, const glm::vec4& color);

		// Line with 2D position
		static void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color);
		// Line with 3D position
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color);

		// Rectangle with 2D position
		static void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		// Rectangle with 3D position
		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

//		DrawRotatedRect(position, size, rotation, color);
//		DrawCircle(position, size, color, filled);
 
		static float SetLineWidth(float width);
 		static float GetLineWidth();
		
//		DrawSprite();

		// Stats
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t LineCount = 0;

			uint32_t GetTotalVertexCount() const { return (QuadCount * 4) + (LineCount * 2); }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static Statistics GetStats();

	private:
		static void StartBatch();
		static void NextBatch();
	};


} // namespace Helios
