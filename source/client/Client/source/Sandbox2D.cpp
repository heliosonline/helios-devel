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
	m_VertexArray = Helios::VertexArray::Create();
	float vertices[(3 + 0) * 4] = {
		-0.5f, -0.5f, 0.0f, // bottom left
		 0.5f, -0.5f, 0.0f, // bottom right
		 0.5f,  0.5f, 0.0f, // top right
		-0.5f,  0.5f, 0.0f  // top left
	};
	Helios::Ref<Helios::VertexBuffer> vb = Helios::VertexBuffer::Create(vertices, sizeof(vertices));
	Helios::BufferLayout layout = {
		{ Helios::ShaderDataType::Float3, "a_Position" },
	};
	vb->SetLayout(layout);
	m_VertexArray->AddVertexBuffer(vb);
	unsigned int indices[6] = {
		0, 1, 2, 2, 3, 0
	};
	Helios::Ref<Helios::IndexBuffer>ib = Helios::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(ib);

	m_FlatColorShader = Helios::Shader::Create("assets/shaders/FlatColorShader.glsl");
}


void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Helios::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Helios::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Helios::RenderCommand::Clear();

	Helios::Renderer::BeginScene(m_CameraController.GetCamera());

	m_FlatColorShader->Bind();
	m_FlatColorShader->SetFloat4("u_Color", m_Color);
	Helios::Renderer::Submit(m_FlatColorShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Helios::Renderer::EndScene();
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
