#include "pch.h"

#include "EditorLayer.h"

#include <imgui.h>


EditorLayer::EditorLayer()
	: Layer("EditorLayer"), m_CameraController(800.0f / 600.0f, true)
{
}


void EditorLayer::OnAttach()
{
	m_Texture = Helios::Texture2D::Create("Assets/Textures/Checkerboard.png");
	m_Texture2 = Helios::Texture2D::Create("Assets/Textures/ChernoLogo.png");
//	m_SubTexture = Helios::SubTexture2D::CreateByIndex(m_Texture, 0, 0, 4, 4);
	m_SubTexture = Helios::SubTexture2D::CreateByCoord(m_Texture, { 0.0f, 0.0f }, { 0.25f, 0.25f });

	Helios::FramebufferSpecification fbs;
	fbs.Width = 800;
	fbs.Height = 600;
	m_Framebuffer = Helios::Framebuffer::Create(fbs);
}


void EditorLayer::OnDetach()
{
}


void EditorLayer::OnUpdate(Helios::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Helios::Renderer2D::ResetStats();
	m_Framebuffer->Bind();
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
	m_Framebuffer->Unbind();
}


void EditorLayer::OnImGuiRender()
{
	static bool dockspaceOpen = true;

	// If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
	// In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
	// In this specific demo, we are not using DockSpaceOverViewport() because:
	// - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
	// - we allow the host window to have padding (when opt_padding == true)
	// - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
	// TL;DR; this demo is more complicated than what you would normally use.
	// If we removed all the options we are showcasing, this demo would become:
	//     void ShowExampleAppDockSpace()
	//     {
	//         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	//     }

//	static bool opt_fullscreen = true;
//	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	//	if (opt_fullscreen)
	//	{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	//	}
	//	else
	//	{
	//		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	//	}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
//	if (!opt_padding)
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	//	if (!opt_padding)
	ImGui::PopStyleVar();

	//	if (opt_fullscreen)
	ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
//			ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
//			ImGui::MenuItem("Padding", NULL, &opt_padding);
//			ImGui::Separator();

			if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
			if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
			if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
			if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
			//			if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
			ImGui::Separator();

			if (ImGui::MenuItem("Close"))
				Helios::Application::Get().Close();

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	ImGui::Begin("Viewport");
	ImTextureID textureID = (ImTextureID)m_Framebuffer->GetColorAttachmentRendererID();
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	if (m_ViewportSize != *((glm::vec2*)&viewportSize))
	{
		m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ViewportSize = { viewportSize.x, viewportSize.y };
	}
	ImGui::Image(textureID, viewportSize, ImVec2{0, 1}, ImVec2{1, 0});
	ImGui::End();
	ImGui::PopStyleVar();

#if 1
	ImGui::Begin("Statistics");
	auto stats = Helios::Renderer2D::GetStats();
	ImGui::Text("%6d Draw Calls", stats.DrawCalls);
	ImGui::Text("%6d Quads", stats.QuadCount);
	ImGui::Text("%6d Lines", stats.LineCount);
	ImGui::Text("%6d Vertices", stats.GetTotalVertexCount());
	ImGui::Text("%6d Indices", stats.GetTotalIndexCount());
	ImGui::End();
#endif
}


void EditorLayer::OnEvent(Helios::Event& e)
{
	m_CameraController.OnEvent(e);
}
