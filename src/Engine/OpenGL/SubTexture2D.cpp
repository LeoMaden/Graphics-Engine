#include "SubTexture2D.h"

namespace Engine {


	SubTexture2D::SubTexture2D(const Texture2D& texture, const SubTexture2DData& data)
		: m_Texture(texture), m_TexCoords()
	{
		glm::vec2 origin = data.Origin;
		origin.x += data.Padding.w; // Offset left padding
		origin.y += data.Padding.z; // Offset bottom padding

		// Offset of origin of subtexture.
		float xOffset = data.SpriteSize.x * data.UnitSize.x * data.Location.x;
		float yOffset = data.SpriteSize.y * data.UnitSize.y * data.Location.y;

		float width = data.SpriteSize.x * data.UnitSize.x - data.Padding.y - data.Padding.w;
		float height = data.SpriteSize.y * data.UnitSize.y - data.Padding.x - data.Padding.z;

		glm::vec2 texCoords[4]{
			{ origin.x + xOffset,			origin.y + yOffset },
			{ origin.x + xOffset + width,	origin.y + yOffset },
			{ origin.x + xOffset + width,	origin.y + yOffset + height },
			{ origin.x + xOffset,			origin.y + yOffset + height },
		};

		// Normalise tex coords
		glm::vec2 texSize(texture.GetWidth(), texture.GetHeight());
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