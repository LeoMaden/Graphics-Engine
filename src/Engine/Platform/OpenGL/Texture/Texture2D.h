#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Utils.h"
#include "Texture2DBase.h"

namespace Engine {

	class Texture2D : public Texture2DBase
	{
	public:
		Texture2D();
		~Texture2D();

		virtual uint32_t GetId() const override { return m_Id; }
		virtual void Bind(uint32_t slot) const override;

		void SetImage(const std::string& path);
		void SetImage(uint8_t* data, const Utils::ImageProps& props);

		int GetWidth()		const { return m_Props.Width; }
		int GetHeight()		const { return m_Props.Height; }
		int GetChannels()	const { return m_Props.Channels; }

		virtual const glm::vec2& GetTexCoord(uint32_t i) const override { return m_TexCoords[i]; }

	private:
		GLuint				m_Id;
		Utils::ImageProps	m_Props;
		glm::vec2			m_TexCoords[4];
	};

}