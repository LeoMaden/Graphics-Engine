#include "Renderer2D.h"

#include <glad/glad.h>
#include <iostream>

namespace Engine {

	/*static*/ void Renderer2D::Setup()
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) 
		{
			std::cout << "Debug enabled" << std::endl;	
			
			auto callback = [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
			{
				std::cout << "GL ERROR: " << message << std::endl;
			};

			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(callback, nullptr);
		}
		else
		{
			std::cout << "Debug not enabled" << std::endl;
		}

		float vertices[]{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f
		};

		unsigned int indices[]{
			0, 1, 2, 2, 3, 0
		};

		GLuint vbo;
		GLuint ibo;
		GLuint vao;

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glCreateBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glCreateBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glEnableVertexArrayAttrib(vao, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

		glEnableVertexArrayAttrib(vao, 1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));

		char* vsSource = R"(
			#version 450 core
			layout (location = 0) in vec3 aPosition;
			layout (location = 1) in vec3 aColor;

			out vec3 oColor;

			void main() 
			{
				oColor = aColor;
				gl_Position = vec4(aPosition, 1.0);
			})";

		char* fsSource = R"(
			#version 450 core
			out vec4 vColor;

			in vec3 oColor;

			void main() 
			{
				vColor = vec4(oColor, 1.0);
			})";

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vsSource, nullptr);
		glCompileShader(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fsSource, nullptr);
		glCompileShader(fs);

		GLuint prog = glCreateProgram();
		glAttachShader(prog, vs);
		glAttachShader(prog, fs); 
		glLinkProgram(prog);
		glUseProgram(prog);
	}

	/*static*/ void Renderer2D::DrawSquare()
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

}