#include "pch.h"

#include "config/version.h"


#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>


class ExampleLayer : public Helios::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(800.0f / 600.0f, true)
	{
		m_VertexArray_1 = Helios::VertexArray::Create();
		float vertices_1[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};
		Helios::Ref<Helios::VertexBuffer> vb_1 = Helios::VertexBuffer::Create(vertices_1, sizeof(vertices_1));
		Helios::BufferLayout layout_1 = {
			{ Helios::ShaderDataType::Float3, "a_Position" },
			{ Helios::ShaderDataType::Float4, "a_Color" }
		};
		vb_1->SetLayout(layout_1);
		m_VertexArray_1->AddVertexBuffer(vb_1);
		unsigned int indices_1[3] = {
			0, 1, 2
		};
		Helios::Ref<Helios::IndexBuffer>ib_1 = Helios::IndexBuffer::Create(indices_1, sizeof(indices_1) / sizeof(uint32_t));
		m_VertexArray_1->SetIndexBuffer(ib_1);


		m_VertexArray_2 = Helios::VertexArray::Create();
		float vertices_2[(3 + 2) * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left
		};
		Helios::Ref<Helios::VertexBuffer> vb_2 = Helios::VertexBuffer::Create(vertices_2, sizeof(vertices_2));
		Helios::BufferLayout layout_2 = {
			{ Helios::ShaderDataType::Float3, "a_Position" },
			{ Helios::ShaderDataType::Float2, "a_TexCoord" }
		};
		vb_2->SetLayout(layout_2);
		m_VertexArray_2->AddVertexBuffer(vb_2);
		unsigned int indices_2[6] = {
			0, 1, 2, 2, 3, 0
		};
		Helios::Ref<Helios::IndexBuffer>ib_2 = Helios::IndexBuffer::Create(indices_2, sizeof(indices_2) / sizeof(uint32_t));
		m_VertexArray_2->SetIndexBuffer(ib_2);

		// shader 1 (triangle) ////////////////////////////////////////////////
		{
			std::string vs_1 = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
			)";
			std::string fs_1 = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position + 0.5, 1.0);
				color = v_Color;
			}
			)";
			m_Shader_1 = Helios::Shader::Create("shader1", vs_1, fs_1);
		}
		// shader 2 (grid) ////////////////////////////////////////////////////
		{
			std::string vs_2 = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				}
			)";
			std::string fs_2 = R"(
				#version 330 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;

				uniform vec3 u_Color;

				void main()
				{
					color = vec4(u_Color, 1.0);
				}
			)";
			m_Shader_2 = Helios::Shader::Create("shader2", vs_2, fs_2);
		}
		// shader 3 (texture) /////////////////////////////////////////////////
		auto texShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Helios::Texture2D::Create("assets/textures/Checkerboard.png");
		m_LogoTexture = Helios::Texture2D::Create("assets/textures/ChernoLogo.png");

		texShader->Bind();
		texShader->SetInt("u_Texture", 0);
	}


	void OnUpdate(Helios::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Helios::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Helios::RenderCommand::Clear();

		Helios::Renderer::BeginScene(m_CameraController.GetCamera());


		// grid
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		m_Shader_2->Bind();
		m_Shader_2->SetFloat3("u_Color", m_Color);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Helios::Renderer::Submit(m_Shader_2, m_VertexArray_2, transform);
			}
		}

		auto texShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Helios::Renderer::Submit(texShader, m_VertexArray_2, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_LogoTexture->Bind();
		Helios::Renderer::Submit(texShader, m_VertexArray_2, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// triangle
		//Helios::Renderer::Submit(m_Shader_1, m_VertexArray_1);

		Helios::Renderer::EndScene();
	}


	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Color", glm::value_ptr(m_Color));
		ImGui::End();
	}


	void OnEvent(Helios::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}


private:
	Helios::ShaderLibrary m_ShaderLibrary;
	Helios::Ref<Helios::Shader> m_Shader_1, m_Shader_2;
	Helios::Ref<Helios::VertexArray> m_VertexArray_1, m_VertexArray_2;
	Helios::Ref<Helios::Texture2D> m_Texture, m_LogoTexture;
	Helios::OrthographicCameraController m_CameraController;
	glm::vec3 m_Color = { 0.2f, 0.3f, 0.8f };
};


class HeliosGame : public Helios::Application
{
public:
	HeliosGame(const Helios::ApplicationSpecification& specification)
		: Helios::Application(specification)
	{
		PushLayer(new ExampleLayer());
	}

	~HeliosGame()
	{
	}
};


Helios::Application* Helios::CreateApplication(Helios::ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "HeliosGame";
	spec.CommandLineArgs = args;
	spec.hints |= Helios::Hints::HINT_USE_EXEPATH_AS_CWD;

	return new HeliosGame(spec);
}
