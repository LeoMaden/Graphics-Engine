#pragma once
#include <vector>
#include <glad/glad.h>

namespace Engine {

	class IndexBuffer
	{
	public:
		IndexBuffer(const std::vector<uint32_t>& indices);

		void Bind() const;

		uint32_t						GetId()			const { return m_Id; }
		const std::vector<uint32_t>&	GetIndices()	const { return m_Indices; }

	private:
		GLuint					m_Id;
		std::vector<uint32_t>	m_Indices;
	};

} 