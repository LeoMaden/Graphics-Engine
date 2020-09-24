#include "Pch.h"
#include "Camera2D.h"

#include "Log.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

	Camera2D::Camera2D()
		: m_Position(0.0f, 0.0f, -1.0f), m_ProjMatrix(1.0f), m_ViewMatrix(1.0f), m_ViewProjMat(1.0f)
	{
	}

	void Camera2D::Move(const glm::vec3& delta)
	{
		m_Position += delta;

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		RecalcViewProj();
	}

	void Camera2D::SetPos(const glm::vec3& position)
	{
		m_Position = position;

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		RecalcViewProj();
	}

	void Camera2D::SetProjection(const glm::vec2& size, const glm::vec2& centre)
	{
		float left = -centre.x;
		float right = size.x - centre.x;

		float bottom = -centre.y;
		float top = size.y - centre.y;

		m_ProjMatrix = glm::ortho(left, right, bottom, top, 0.0f, 100.0f);
		RecalcViewProj();
	}

	void Camera2D::RecalcViewProj()
	{
		m_ViewProjMat = m_ProjMatrix * m_ViewMatrix;
	}
}