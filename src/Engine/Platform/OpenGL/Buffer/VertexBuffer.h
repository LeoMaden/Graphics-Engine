#pragma once
#include <vector>
#include <glad/glad.h>

#include "Renderer/RenderUtils.h"
#include "BufferType.h"

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
		// ----- Static VertexBuffers -----

		// Create a VertexBuffer with fixed data.
		// Size in bytes.
		VertexBuffer(const void* data, uint32_t size);


		// ----- Dynamic VertexBuffers -----

		// Create a VertexBuffer with a given capacity (in bytes)
		// which can have data written to it at will.
		VertexBuffer(uint32_t capacity);

		// Set the data in a dynamic VertexBuffer.
		// Size is given in bytes.
		void SetData(const void* data, uint32_t size);


		// ----- All VertexBuffers -----
		~VertexBuffer();

		void AddLayout(uint32_t location, DataType type, uint32_t count);

		uint32_t					GetId()		const { return m_Id; }
		const std::vector<Layout>&	GetLayout()	const { return m_Layout; }
		BufferType					GetType()	const { return m_Type; }

	private:
		GLuint					m_Id;
		uint32_t				m_Capacity;
		std::vector<Layout>		m_Layout;
		BufferType				m_Type;

	};

}