#pragma once
#include <glad/glad.h>

#include "Utils.h"

namespace Engine {

	class Texture2D
	{
	public:
		Texture2D();
		~Texture2D();

		uint32_t GetId() const { return m_Id; }
		void Bind(uint32_t slot) const;

		void SetImage(const std::string& path);
		void SetImage(uint8_t* data, const Utils::ImageProps& props);

	private:
		GLuint m_Id;
	};

}