#pragma once

namespace Engine
{
	enum class ShaderDataType
	{
		Float, Int, UInt, Int16, UInt16, Float64, Vector2, Vector3, Vector4, Matrix2, Matrix3, Matrix4
	};

	uint32_t GetShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Float:     return sizeof(float_t);
		case Engine::ShaderDataType::Int:		return sizeof(int32_t);
		case Engine::ShaderDataType::UInt:		return sizeof(uint32_t);
		case Engine::ShaderDataType::Int16:		return sizeof(int16_t);
		case Engine::ShaderDataType::UInt16:	return sizeof(uint16_t);
		case Engine::ShaderDataType::Float64:	return sizeof(double_t);
		case Engine::ShaderDataType::Vector2:	return 2 * sizeof(float_t);
		case Engine::ShaderDataType::Vector3:	return 3 * sizeof(float_t);
		case Engine::ShaderDataType::Vector4:	return 4 * sizeof(float_t);
		case Engine::ShaderDataType::Matrix2:	return 2 * 2 * sizeof(float_t);
		case Engine::ShaderDataType::Matrix3:	return 3 * 3 * sizeof(float_t);
		case Engine::ShaderDataType::Matrix4:	return 4 * 4 * sizeof(float_t);

		default:
			ASSERT(false, "Data type not supported");
			return 0;
		}
	}
}