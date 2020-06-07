#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <unordered_map>

namespace Engine {

	class Shader
	{
	public:
		Shader();

		void AddVertexShader(const std::string& path);
		void AddFragmentShader(const std::string& path);

		void Link() const;
		void Bind() const;

		void SetVec3(const std::string& name, const glm::vec3& vec);
		void SetVec4(const std::string& name, const glm::vec4& vec);

	protected:
		void AddShader(const std::string& path, GLenum type);
		GLint GetLocation(const std::string& name) const;

	private:
		GLuint					m_ProgId;
		std::vector<GLuint>		m_ShaderIds;

		mutable std::unordered_map<std::string, GLuint> m_UniformCache;
	};

}