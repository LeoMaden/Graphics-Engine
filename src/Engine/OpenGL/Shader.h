#pragma once
#include <glad/glad.h>
#include <vector>
#include <string>

namespace Engine {

	class Shader
	{
	public:
		Shader();

		void AddVertexShader(const std::string& path);
		void AddFragmentShader(const std::string& path);

		void Link() const;
		void Bind() const;

	protected:
		void AddShader(const std::string& path, GLenum type);

	private:
		GLuint					m_ProgId;
		std::vector<GLuint>		m_ShaderIds;
	};

}