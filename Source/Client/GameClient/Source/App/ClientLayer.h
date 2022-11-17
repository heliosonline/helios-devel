#pragma once

#include <HeliosEngine/HeliosEngine.h>


class ClientLayer : public Helios::Layer
{
public:
	ClientLayer();
	~ClientLayer() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Helios::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Helios::Event& e) override;

private:
	Helios::OrthographicCameraController m_CameraController;

	// Temp
	Ref<Helios::VertexArray> m_VertexArray;
	Ref<Helios::Shader> m_FlatColorShader;
	Ref<Helios::Framebuffer> m_Framebuffer;
	Ref<Helios::Texture2D> m_Texture;
	Ref<Helios::Texture2D> m_Texture2;
	Ref<Helios::SubTexture2D> m_SubTexture;
};
