#pragma once
#include <vector>
#include <glad/glad.h>

namespace Engine {

	class IndexBuffer
	{
	public:
		IndexBuffer(uint32_t capacity);
		~IndexBuffer();

		void Bind() const;
		void SetIndices(void* indices, uint32_t size);

		uint32_t GetId() const { return m_Id; }

	private:
		GLuint					m_Id;
		uint32_t				m_Capacity;
	};

} 