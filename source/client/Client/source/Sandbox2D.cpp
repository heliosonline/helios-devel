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

	Helios::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Helios::RenderCommand::Clear();

	Helios::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Helios::Renderer2D::DrawQuad({ -0.5f, 0.0f, 0.1f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Helios::Renderer2D::DrawQuad({ 0.5f, -0.5f, -0.1 }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Helios::Renderer2D::DrawRotatedQuad({ 0.2f, 0.0f }, { 1.0f, 1.0f }, 45.0f, m_Texture, { 1.0f, 1.0f, 1.0f, 0.5f });
	Helios::Renderer2D::DrawQuad({ -0.5f, 0.5f, 0.2f }, { 0.5f, 0.5f }, m_Texture2, { 1.0f, 1.0f, 1.0f, 0.75f });
	Helios::Renderer2D::EndScene();
}


void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_Color));
	ImGui::End();
}


void Sandbox2D::OnEvent(Helios::Event& e)
{
	m_CameraController.OnEvent(e);
}
