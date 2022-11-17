#include "pch.h"

#include "Platform/Renderer/OpenGL/GLShader.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>


namespace Helios {


	namespace Utils {


		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			//LOG_CORE_ASSERT(false, "GLShader: Unknown shader type!");
			return 0;
		}


	} // namespace Utils


	GLShader::GLShader(const std::string& filepath)
		: m_FilePath(filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}


	GLShader::GLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}


	GLShader::~GLShader()
	{
		glDeleteProgram(m_RendererID);
	}


	void GLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}


	void GLShader::Unbind() const
	{
		glUseProgram(0);
	}


	void GLShader::SetInt(const std::string& name, int value)
		{ UploadUniformInt(name, value); }
	void GLShader::SetIntArray(const std::string& name, int* values, uint32_t count)
		{ UploadUniformIntArray(name, values, count); }
	void GLShader::SetFloat(const std::string& name, float value)
		{ UploadUniformFloat(name, value); }
	void GLShader::SetFloat2(const std::string& name, const glm::vec2& value)
		{ UploadUniformFloat2(name, value); }
	void GLShader::SetFloat3(const std::string& name, const glm::vec3& value)
		{ UploadUniformFloat3(name, value); }
	void GLShader::SetFloat4(const std::string& name, const glm::vec4& value)
		{ UploadUniformFloat4(name, value); }
	void GLShader::SetMat4(const std::string& name, const glm::mat4& value)
		{ UploadUniformMat4(name, value); }


	GLint GLShader::GetUniformLocation(const std::string& name) const
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		m_UniformLocationCache[name] = location;
		return location;
	}


	void GLShader::UploadUniformInt(const std::string& name, int value)
		{ glUniform1i(GetUniformLocation(name), value); }
	void GLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
		{ glUniform1iv(GetUniformLocation(name), count, values); }
	void GLShader::UploadUniformFloat(const std::string& name, float value)
		{ glUniform1f(GetUniformLocation(name), value); }
	void GLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
		{ glUniform2f(GetUniformLocation(name), value.x, value.y); }
	void GLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
		{ glUniform3f(GetUniformLocation(name), value.x, value.y, value.z); }
	void GLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
		{ glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w); }
	void GLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
		{ glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)); }
	void GLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
		{ glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix)); }


	std::string GLShader::ReadFile(const std::string& filepath)
	{
		std::string result;

		// open file
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (!in)
		{
			LOG_CORE_ERROR("GLShader: Could not open file '{0}'", filepath);
			return "";
		}

		// read file
		in.seekg(0, std::ios::end);
		size_t size = in.tellg();
		if (size == -1)
		{
			LOG_CORE_ERROR("GLShader: Could not read from file '{0}'", filepath);
			return "";
		}
		result.resize(size);
		in.seekg(0, std::ios::beg);
		in.read(&result[0], size);
		return result;
	}


	std::unordered_map<GLenum, std::string> GLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type ";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); // start of shader type declaration line
		while (pos != std::string::npos)
		{
			// end of shader type declaration line
			size_t eol = source.find_first_of("\r\n", pos);
			LOG_CORE_ASSERT(eol != std::string::npos, "GLShader: Syntax error! No source after type declaration.");

			// start of shader type name (after "#type " keyword + any count of spaces)
			size_t begin = source.find_first_not_of(" ", pos + typeTokenLength);

			// typestring until eol or first space
			std::string type = source.substr(begin, source.find_first_of(" \r\n", begin) - begin);
			LOG_CORE_ASSERT(Utils::ShaderTypeFromString(type), "GLShader: Invalid shader type specified!");

			// start of shader code after shader type declaration line
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			LOG_CORE_ASSERT(nextLinePos != std::string::npos, "GLShader: Syntax error! No source after type declaration.");

			// start of next shader type declaration line
			pos = source.find(typeToken, nextLinePos);

			// extract the source
			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}


	void GLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		LOG_CORE_ASSERT(shaderSources.size() <= 2, "GLShader: We only support max 2 shader now!");
		std::array<GLenum, 2> glShaderIDs = { 0, 0 };
		int glShaderIDIndex = 0;

		// Get a program object.
		GLuint program = glCreateProgram();

		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			// Create an empty shader handle
			GLuint shader = glCreateShader(type);

			// Send the shader source code to GL
			const GLchar* c_source = source.c_str();
			glShaderSource(shader, 1, &c_source, 0);

			// Compile the shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// cleanup
				glDeleteShader(shader);

				LOG_CORE_ERROR("GLShader: {0}", infoLog.data());
				LOG_CORE_ASSERT(false, "GLShader: Shader compilation failure!");
				return;
			}

			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// cleanup
			glDeleteProgram(program);
			for (auto id : glShaderIDs)
			{
				if (id)
					glDeleteShader(id);
			}

			LOG_CORE_ERROR("GLShader: {0}", infoLog.data());
			LOG_CORE_ASSERT(false, "GLShader: Shader link failure!");
			return;
		}

		// cleanup
		for (auto id : glShaderIDs)
		{
			if (id)
				glDetachShader(program, id);
		}

		// save the shader ID
		m_RendererID = program;
	}


} // namespace Helios
