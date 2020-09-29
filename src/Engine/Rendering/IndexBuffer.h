#pragma once
#include "ShaderDataType.h"

#include <cstdint>

namespace Engine {

	class IndexBuffer
	{	
		// Static
	public:
		// Create a new vertex buffer of a given size and optionally fill it with data.
		static IndexBuffer* Create(const void* indices, uint32_t size, BufferUsage usage);

		// Instance
	public:
		virtual int GetId() const = 0;

		virtual void SetIndexDataType(ShaderDataType type) = 0;
		virtual ShaderDataType GetIndexDataType() const = 0;

		virtual void SetIndices(const void* indices, uint32_t size) = 0;
	};

}