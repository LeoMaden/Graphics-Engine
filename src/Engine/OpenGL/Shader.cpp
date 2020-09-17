#include "Shader.h"
#include "Utils.h"
#include "Log.h"

#include <glm/gtc/type_ptr.hpp>

namespace Engine {

	Shader::Shader()
		: m_ProgId(0), m_ShaderIds()
		, m_UniformCache()
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


	void Shader::SetFloat(const std::string& name, float val)
	{
		GLint loc = GetLocation(name);
		glUniform1f(loc, val);
	}

	void Shader::SetVec3(const std::string& name, const glm::vec3& vec)
	{
		GLint loc = GetLocation(name);
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}

	void Shader::SetVec4(const std::string& name, const glm::vec4& vec)
	{
		GLint loc = GetLocation(name);
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& mat)
	{
		GLint loc = GetLocation(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void Shader::SetInt(const std::string& name, int val)
	{
		GLint loc = GetLocation(name);
		glUniform1i(loc, val);
	}

	void Shader::SetIntArray(const std::string& name, int* vals, uint32_t count)
	{
		GLint loc = GetLocation(name);
		glUniform1iv(loc, count, vals);
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

	GLint Shader::GetLocation(const std::string& name) const
	{
		if (m_UniformCache.find(name) == m_UniformCache.end())
		{
			// Not found.
			GLint loc = glGetUniformLocation(m_ProgId, name.c_str());

			// Invalid location
			if (loc == -1)
			{
				LOG_WARN("Uniform {} not found", name);
			}

			m_UniformCache[name] = loc;
			return loc;
		}

		return m_UniformCache[name];
	}

}