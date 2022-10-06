#include "pch.h"

#include "HeliosEngine/Renderer/Renderer.h"
#include "HeliosEngine/Renderer/Shader.h"

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
#	include "Platform/Renderer/DirectX/DXShader.h"
#endif
#ifdef BUILDWITH_RENDERER_METAL
#	include "Platform/Renderer/Metal/MTShader.h"
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
#	include "Platform/Renderer/Vulkan/VKShader.h"
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
#	include "Platform/Renderer/OpenGL/GLShader.h"
#endif


namespace Helios {


	//////////////////////////////////////////////////////////////////////////////
	// Shader::Create(...) ///////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////


	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX:  return CreateRef<DXShader>(filepath);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal:  return CreateRef<MTShader>(filepath);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan:  return CreateRef<VKShader>(filepath);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:  return CreateRef<GLShader>(filepath);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    LOG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;

// related on build options and platform
#ifdef BUILDWITH_RENDERER_DIRECTX
			case RendererAPI::API::DirectX:  return CreateRef<DXShader>(name, vertexSrc, fragmentSrc);
#endif
#ifdef BUILDWITH_RENDERER_METAL
			case RendererAPI::API::Metal:  return CreateRef<MTShader>(name, vertexSrc, fragmentSrc);
#endif
#ifdef BUILDWITH_RENDERER_VULKAN
			case RendererAPI::API::Vulkan:  return CreateRef<VKShader>(name, vertexSrc, fragmentSrc);
#endif
#ifdef BUILDWITH_RENDERER_OPENGL
			case RendererAPI::API::OpenGL:  return CreateRef<GLShader>(name, vertexSrc, fragmentSrc);
#endif

			default: LOG_CORE_ASSERT(false, "Unknown RendererAPI!"); return nullptr;
		}
	}


	//////////////////////////////////////////////////////////////////////////////
	// ShaderLibrary /////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		LOG_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}


	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}


	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}


	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}


	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		LOG_CORE_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}


	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}


} // namespace Helios
