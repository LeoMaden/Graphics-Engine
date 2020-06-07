#include "Shader.h"
#include "Utils.h"

namespace Engine {

	Shader::Shader()
		: m_ProgId(0), m_ShaderIds()
	{
		m_ProgId = glCreateProgram();
	}

	void Shader::AddVertexShader(const std::string& path)
	{
		AddShader(path, GL_VERTEX_SHADER);
	}

	void Shader::AddFragmentShader(const std::string& path)
	{
		AddShader(path, GL_FRAGMENT_SHADER);
	}

	void Shader::Link() const
	{
		glLinkProgram(m_ProgId);

		for (GLuint id : m_ShaderIds)
		{
			glDeleteShader(id);
		}
	}

	void Shader::Bind() const
	{
		glUseProgram(m_ProgId);
	}

	void Shader::AddShader(const std::string& path, GLenum type)
	{
		std::string srcStr = Utils::LoadFile(path);
		const char* src = srcStr.c_str();

		GLuint id = glCreateShader(type);
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		glAttachShader(m_ProgId, id);
		m_ShaderIds.push_back(id);
	}

}