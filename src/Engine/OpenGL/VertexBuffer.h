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
		VertexBuffer(uint32_t capacity);
		~VertexBuffer();

		void Bind() const;
		void AddLayout(uint32_t location, GLenum type, uint32_t count);
		void SetData(void* data, uint32_t size);

		uint32_t					GetId()		const { return m_Id; }
		const std::vector<Layout>&	GetLayout()	const { return m_Layout; }

	private:
		GLuint					m_Id;
		uint32_t				m_Capacity;
		std::vector<Layout>		m_Layout;
	};

}