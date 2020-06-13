#include "SubTexture2D.h"

namespace Engine {

	SubTexture2D::SubTexture2D(const Texture2D& texture, const glm::vec2& location, const glm::vec2 unitSize, const glm::vec2& spriteSize)
		: m_Texture(texture), m_TexCoords()
	{
		glm::vec2	origin	= location * unitSize;
		float		width	= unitSize.x * spriteSize.x;
		float		height	= unitSize.y * spriteSize.y;
		glm::vec2	texSize = glm::vec2(texture.GetWidth(), texture.GetHeight());

		glm::vec2 texCoords[4]{
			origin,
			origin + glm::vec2(width, 0.0f),
			origin + glm::vec2(width, height),
			origin + glm::vec2(0.0f, height)
		};

		m_TexCoords[0] = texCoords[0] / texSize;
		m_TexCoords[1] = texCoords[1] / texSize;
		m_TexCoords[2] = texCoords[2] / texSize;
		m_TexCoords[3] = texCoords[3] / texSize;
	}

	void SubTexture2D::Bind(uint32_t slot) const
	{
		m_Texture.Bind(slot);
	}

}