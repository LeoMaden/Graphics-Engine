#pragma once
#include <vector>
#include <glad/glad.h>

namespace Engine {

	class VertexBuffer
	{
	public:
		struct Layout
		{
			uint32_t	Location;
			GLenum		Type;
			uint32_t	Count;
		};

	public:
		VertexBuffer(const std::vector<float>& data);

		void Bind() const;
		void AddLayout(uint32_t location, GLenum type, uint32_t count);

		uint32_t					GetId()		const { return m_Id; }
		const std::vector<float>&	GetData()	const { return m_Data; }
		const std::vector<Layout>&	GetLayout()	const { return m_Layout; }

	private:
		GLuint					m_Id;
		std::vector<float>		m_Data;
		std::vector<Layout>		m_Layout;
	};

}