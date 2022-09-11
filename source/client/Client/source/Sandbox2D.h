#pragma once


class Sandbox2D : public Helios::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;

	void OnAttach() override;
	void OnDetach() override;

	void OnUpdate(Helios::Timestep ts) override;
	void OnImGuiRender() override;
	void OnEvent(Helios::Event& e) override;

private:
	Helios::OrthographicCameraController m_CameraController;

	// Temp
	Helios::Ref<Helios::VertexArray> m_VertexArray;
	Helios::Ref<Helios::Shader> m_FlatColorShader;
	Helios::Ref<Helios::Texture2D> m_Texture;
	Helios::Ref<Helios::Texture2D> m_Texture2;
	Helios::Ref<Helios::SubTexture2D> m_SubTexture;

	glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };

};
