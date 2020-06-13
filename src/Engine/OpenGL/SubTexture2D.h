#pragma once
#include "Texture2D.h"
#include "Texture2DBase.h"

namespace Engine {

	class SubTexture2D : public Texture2DBase
	{
	public:
		SubTexture2D(const Texture2D& texture, 
			const glm::vec2& location, 
			const glm::vec2 unitSize, 
			const glm::vec2& spriteSize = { 1.0f, 1.0f });

		virtual uint32_t GetId()							const override { return m_Texture.GetId(); }
		virtual void Bind(uint32_t slot)					const override;
		virtual const glm::vec2& GetTexCoord(uint32_t i)	const override { return m_TexCoords[i]; }

	private:
		const Texture2D& m_Texture;
		glm::vec2 m_TexCoords[4];
	};

}