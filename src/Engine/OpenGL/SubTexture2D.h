#pragma once
#include "Texture2D.h"
#include "Texture2DBase.h"

namespace Engine {

	class SubTexture2D : public Texture2DBase
	{
	public:
		struct SubTexture2DData
		{
			glm::vec2 Location;
			glm::vec2 UnitSize;
			glm::vec4 Padding; // Top right bottom left
			glm::vec2 Origin;
			glm::vec2 SpriteSize = { 1.0f, 1.0f };
		};

		SubTexture2D(const Texture2D& texture, const SubTexture2DData& data);

		virtual uint32_t GetId()							const override { return m_Texture.GetId(); }
		virtual void Bind(uint32_t slot)					const override;
		virtual const glm::vec2& GetTexCoord(uint32_t i)	const override { return m_TexCoords[i]; }

	private:
		const Texture2D& m_Texture;
		glm::vec2 m_TexCoords[4];
	};

}