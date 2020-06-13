#include "Texture2D.h"

#include "Log.h"

#undef LoadImage

namespace Engine {

	Texture2D::Texture2D()
	{
		glGenTextures(1, &m_Id);
		glBindTexture(GL_TEXTURE_2D, m_Id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_Id);
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}

	void Texture2D::SetImage(const std::string& path)
	{
		uint8_t* data = Utils::LoadImage(path, &m_Props);
		SetImage(data, m_Props);
		Utils::FreeImage(data);
	}

	void Texture2D::SetImage(uint8_t* data, const Utils::ImageProps& props)
	{
		GLint format = GL_RGB;

		if (props.Channels == 3)
		{
			format = GL_RGB;
		}
		else if (props.Channels == 4)
		{
			format = GL_RGBA;
		}
		else
		{
			ASSERT(false, "Format unsupported");
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, props.Width, props.Height, 0, format, GL_UNSIGNED_BYTE, data);
	}




}