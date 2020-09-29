#pragma once
#include "VertexBufferLayout.h"
#include "BufferUsage.h"
#include <cstdint>

namespace Engine {

	class VertexBuffer
	{
	// Static
	public:
		// Create a new vertex buffer of a given size and optionally fill it with data.
		static VertexBuffer* Create(const void* data, uint32_t size, BufferUsage usage);

	// Instance
	public:
		virtual int GetId() const = 0;

		virtual void SetLayout(const VertexBufferLayout& layout) = 0;
		virtual VertexBufferLayout GetLayout() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

	};

}