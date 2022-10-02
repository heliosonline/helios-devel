#include "pch.h"

#include "Sandbox2D.h"

#include <imgui.h>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(800.0f / 600.0f, true)
{
}


void Sandbox2D::OnAttach()
{
	m_Texture = Helios::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_Texture2 = Helios::Texture2D::Create("Assets/Textures/ChernoLogo.png");
//	m_SubTexture = Helios::SubTexture2D::CreateByIndex(m_Texture, 0, 0, 4, 4);
	m_SubTexture = Helios::SubTexture2D::CreateByCoord(m_Texture, { 0.0f, 0.0f }, { 0.25f, 0.25f });
}


void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Helios::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Helios::Renderer2D::ResetStats();
	Helios::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Helios::RenderCommand::Clear();

	static float rotation = 0.0f;
	rotation += ts * 20.0f;

	Helios::Renderer2D::BeginScene(m_CameraController.GetCamera());

	/* red     */Helios::Renderer2D::DrawQuad({-0.5f, 0.0f, 0.1f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
	/* blue    */Helios::Renderer2D::DrawQuad({0.5f, -0.5f, -0.1f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
	/* checker */Helios::Renderer2D::DrawRotatedQuad({0.0f, -0.5f}, {1.0f, 1.0f}, rotation, m_Texture, {1.0f, 1.0f, 1.0f, 0.5f});
	/* logo    */Helios::Renderer2D::DrawQuad({-0.5f, 0.5f, 0.2f}, {0.5f, 0.5f}, m_Texture2, {1.0f, 1.0f, 1.0f, 0.75f});
	/* checker */Helios::Renderer2D::DrawRotatedQuad({ 0.0f, 0.5f, 0.05f }, { 1.0f, 1.0f }, rotation, m_SubTexture, { 1.0f, 1.0f, 1.0f, 0.5f });

	float z = -0.05f;
	for (float y = 2.0f; y > -2.0f; y -= 0.2f)
		Helios::Renderer2D::DrawLine({ -2.0f, y, z }, { 2.0f, y, z }, { 1.0f, 0.0f, 0.0f, 1.0f });
	Helios::Renderer2D::DrawRect({ 0.0f, 0.0f, z+0.01f }, { 2.0f, 2.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });

	Helios::Renderer2D::EndScene();

#if 0
	Helios::Renderer2D::BeginScene(m_CameraController.GetCamera());
	float s = 0.1f;
	for (float x = -10.0f; x < 10.0f; x += s)
	{
		for (float y = -10.0f; y < 10.0f; y += s)
		{
			glm::vec4 color = { (x + 10.0f) / 20.0f, (y + 10.0f) / 20.0f, 1.0f, 1.0f };
			Helios::Renderer2D::DrawQuad({ x, y, -0.5f }, { s/2, s/2 }, color);
		}
	}
	Helios::Renderer2D::EndScene();
#endif
}


void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Statistics");
	auto stats = Helios::Renderer2D::GetStats();
	ImGui::Text("%6d Draw Calls", stats.DrawCalls);
	ImGui::Text("%6d Quads", stats.QuadCount);
	ImGui::Text("%6d Lines", stats.LineCount);
	ImGui::Text("%6d Vertices", stats.GetTotalVertexCount());
	ImGui::Text("%6d Indices", stats.GetTotalIndexCount());
	ImGui::End();
}


void Sandbox2D::OnEvent(Helios::Event& e)
{
	m_CameraController.OnEvent(e);
}
