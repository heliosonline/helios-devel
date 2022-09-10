#include "pch.h"

#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(800.0f / 600.0f, true)
{
}


void Sandbox2D::OnAttach()
{
	m_Texture = Helios::Texture2D::Create("assets/textures/Checkerboard.png");
	m_Texture2 = Helios::Texture2D::Create("assets/textures/ChernoLogo.png");
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
	Helios::Renderer2D::DrawQuad({ -0.5f, 0.0f, 0.1f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Helios::Renderer2D::DrawQuad({ 0.5f, -0.5f, -0.1 }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Helios::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, m_Texture, { 1.0f, 1.0f, 1.0f, 0.5f });
	Helios::Renderer2D::DrawQuad({ -0.5f, 0.5f, 0.2f }, { 0.5f, 0.5f }, m_Texture2, { 1.0f, 1.0f, 1.0f, 0.75f });
//	Helios::Renderer2D::EndScene();

//	Helios::Renderer2D::BeginScene(m_CameraController.GetCamera());
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
}


void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Statistics");
	auto stats = Helios::Renderer2D::GetStats();
	ImGui::Text("Draw Calls; %d", stats.DrawCalls);
	ImGui::Text("Quads; %d", stats.QuadCount);
	ImGui::Text("Vertices; %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices; %d", stats.GetTotalIndexCount());
	ImGui::End();
}


void Sandbox2D::OnEvent(Helios::Event& e)
{
	m_CameraController.OnEvent(e);
}
