#include "Camera3D.h"

#include "Log.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

	Camera3D::Camera3D()
		: m_Position(0.0f, 0.0f, 0.0f), m_ProjMatrix(1.0f), m_ViewMatrix(1.0f), m_ViewProjMat(1.0f)
	{
	}

	void Camera3D::Move(const glm::vec3& delta)
	{
		m_Position += delta;

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		RecalcViewProj();
	}

	void Camera3D::SetPos(const glm::vec3& position)
	{
		m_Position = position;

		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		RecalcViewProj();
	}

	void Camera3D::SetProjection(float fov, float aspect)
	{
		m_ProjMatrix = glm::perspective(fov, aspect, 0.1f, 100.0f);
		RecalcViewProj();
	}

	void Camera3D::RecalcViewProj()
	{
		m_ViewProjMat = m_ProjMatrix * m_ViewMatrix;
	}
}