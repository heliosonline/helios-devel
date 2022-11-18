#pragma once

#include "HeliosEngine/Renderer/Shader.h"

// TODO: REMOVE!
typedef unsigned int GLenum;
typedef int GLint;


namespace Helios {


	class GLShader : public Shader
	{
	public:
		GLShader(const std::string& name);
		GLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~GLShader();

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat2(const std::string& name, const glm::vec2& value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		const std::string& GetName() const override { return m_Name; }
	private:
		GLint GetUniformLocation(const std::string& name) const;
		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);
		void UploadUniformMat3(const std::string& name, const glm::mat3& value);
		void UploadUniformMat4(const std::string& name, const glm::mat4& value);
	private:
		std::string ReadFile(const std::string& filepath);

//		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
//		void CompileOrGetOpenGLBinaries();
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
//		void CreateProgram();
//		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
	private:
		uint32_t m_RendererID;
//		std::string m_FilePath;
		std::string m_Name;
		mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

//		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
//		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

//		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
	};


} // namespace Helios
