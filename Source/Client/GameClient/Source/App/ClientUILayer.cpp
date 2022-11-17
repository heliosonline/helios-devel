#include "pch.h"

#include "ClientUILayer.h"


ClientUILayer::ClientUILayer()
	: UILayer()
{
}


void ClientUILayer::OnAttach()
{
	UILayer::OnAttach();
}


void ClientUILayer::OnDetach()
{
	UILayer::OnDetach();
}


void ClientUILayer::OnUpdate(Helios::Timestep ts)
{
	UILayer::OnUpdate(ts);

	Helios::RenderCommand::ClearDepthBuffer();

	Helios::Renderer2D::BeginScene(GetCamera());
	Helios::Renderer2D::DrawRect({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.8f, 0.2f, 1.0f });
	Helios::Renderer2D::EndScene();
}


void ClientUILayer::OnEvent(Helios::Event& e)
{
	UILayer::OnEvent(e);
}
