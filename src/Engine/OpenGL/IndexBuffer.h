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
		void SetIndices(const std::vector<uint32_t>& indices);

		uint32_t						GetId()			const { return m_Id; }
		const std::vector<uint32_t>&	GetIndices()	const { return m_Indices; }

	private:
		GLuint					m_Id;
		uint32_t				m_Capacity;
		std::vector<uint32_t>	m_Indices;
	};

} 