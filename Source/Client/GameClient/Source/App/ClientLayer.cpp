#include "pch.h"

#include "ClientLayer.h"

#include <imgui.h>


ClientLayer::ClientLayer()
	: Layer("ClientLayer"), m_CameraController(800.0f / 600.0f, true)
{
}


void ClientLayer::OnAttach()
{
	m_Texture = Helios::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_Texture2 = Helios::Texture2D::Create("Assets/Textures/ChernoLogo.png");
//	m_SubTexture = Helios::SubTexture2D::CreateByIndex(m_Texture, 0, 0, 4, 4);
	m_SubTexture = Helios::SubTexture2D::CreateByCoord(m_Texture, { 0.0f, 0.0f }, { 0.25f, 0.25f });

//	Helios::FramebufferSpecification fbs;
//	fbs.Width = 800;
//	fbs.Height = 600;
//	m_Framebuffer = Helios::Framebuffer::Create(fbs);
}


void ClientLayer::OnDetach()
{
}


void ClientLayer::OnUpdate(Helios::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Helios::Renderer2D::ResetStats();

//	m_Framebuffer->Bind();

	Helios::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
	Helios::RenderCommand::Clear();

	static float rotation = 0.0f;
	rotation += ts * 20.0f;

	Helios::Renderer2D::BeginScene(m_CameraController.GetCamera());


	// grid
	float z = -0.05f;
	for (float y = 2.0f; y > -2.0f; y -= 0.2f)
		Helios::Renderer2D::DrawLine({ -2.0f, y, z }, { 2.0f, y, z }, { 1.0f, 0.0f, 0.0f, 1.0f });
	Helios::Renderer2D::DrawRect({ 0.0f, 0.0f, z + 0.01f }, { 2.0f, 2.0f }, { 1.0f, 1.0f, 0.0f, 1.0f });

	/* red     */Helios::Renderer2D::DrawQuad({ -0.5f,  0.0f,  0.1f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	/* blue    */Helios::Renderer2D::DrawQuad({ 0.5f, -0.5f, -0.1f }, { 0.5f, 0.7f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	/* circle  */Helios::Renderer2D::DrawCircle({ -0.5f, -0.5f,  0.2f }, { 0.5f, 0.8f }, { 0.2f, 0.7f, 0.2f, 1.0f }, 0.05f);
	/* circle  */Helios::Renderer2D::DrawCircle({ -0.5f, -0.5f,  0.3f }, { 0.8f, 0.5f }, { 0.7f, 0.7f, 0.2f, 1.0f }, 0.05f);
	/* circle  */Helios::Renderer2D::DrawCircle({ -0.5f, -0.5f,  0.3f }, { 0.4f, 0.4f }, { 0.7f, 0.2f, 0.7f, 0.5f }, 1.0f);

	/* checker */Helios::Renderer2D::DrawQuad({ 0.0f, -0.5f }, { 1.0f, 1.0f }, rotation, m_Texture, { 1.0f, 1.0f, 1.0f, 0.5f });
	/* logo    */Helios::Renderer2D::DrawQuad({ -0.5f,  0.5f, 0.2f }, { 0.5f, 0.5f }, m_Texture2, { 1.0f, 1.0f, 1.0f, 0.7f });
	/* checker */Helios::Renderer2D::DrawQuad({ 0.0f,  0.5f, 0.05f }, { 1.0f, 1.0f }, rotation, m_SubTexture, { 1.0f, 1.0f, 1.0f, 0.5f });

	float t_size = 0.12f;
	float t_rot = 0.0f;
	float t_maxlength = 4.0f;
	Ref<Helios::Font> font = Helios::FontLibrary::Get("OpenSans", Helios::FontFlags::Regular);
	Helios::Renderer2D::DrawQuad({ 1.0f,  1.5f, 0.0f }, { 1.0f, 1.0f }, font->GetAtlasTexture(), {1.0f, 1.0f, 1.0f, 1.0f});
//	std::string text = "Lorem ipsum dolor sit amet, consectetur adipisci elit, sed eiusmod tempor incidunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur. Quis aute iure reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint obcaecat cupiditat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
//	Helios::Renderer2D::DrawString(font, text, { 0.0f, 0.1f, 0.2f }, t_size, t_maxlength, glm::vec4(1.0f), t_rot);
//	Helios::Renderer2D::DrawString(font, text, { -0.5f, 0.1f, 0.2f }, t_size/10, t_maxlength/10, glm::vec4(1.0f), t_rot);

	Helios::Renderer2D::DrawString(font, "The quick brown @ jumped (1.0)", { -0.5f, 2.0f, 0.2f }, 1.0f, glm::vec4(1.0f));
	float t_posy = 1.0f;
	while (t_size > 0.01f)
	{
		std::string test_text = "The quick brown @ jumped ";
		test_text.append(std::to_string(t_size));

		Helios::Renderer2D::DrawString(font, test_text, { -0.5f, t_posy, 0.2f }, t_size, glm::vec4(1.0f));

		t_posy -= t_size;
		t_size -= 0.005f;
	}


	Helios::Renderer2D::EndScene();
//	m_Framebuffer->Unbind();
}


void ClientLayer::OnImGuiRender()
{
	ImGui::Begin("Statistics");
	auto stats = Helios::Renderer2D::GetStats();
	ImGui::Text("%6d Draw Calls", stats.DrawCalls);
	ImGui::Text("%6d Quads", stats.QuadCount);
	ImGui::Text("%6d Lines", stats.LineCount);
	ImGui::Text("%6d Circles", stats.CircleCount);
	ImGui::Text("%6d Glyphs", stats.GlyphCount);
	ImGui::Text("%6d Vertices", stats.GetTotalVertexCount());
	ImGui::Text("%6d Indices", stats.GetTotalIndexCount());

//	uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
//	ImGui::Image((ImTextureID)textureID, ImVec2{ 400.0f, 300.0f }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
}


void ClientLayer::OnEvent(Helios::Event& e)
{
	m_CameraController.OnEvent(e);
}
