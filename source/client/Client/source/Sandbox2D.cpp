#include "pch.h"

#include "Sandbox2D.h"

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Example"), m_CameraController(800.0f / 600.0f, true)
{
}


void Sandbox2D::OnAttach()
{
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
	Helios::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Helios::Renderer2D::EndScene();

//	m_FlatColorShader->SetFloat4("u_Color", m_Color);
//	Helios::Renderer::Submit(m_FlatColorShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
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
