#include "Camera3D.h"

#include "Log.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

	Camera3D::Camera3D()
		: m_Position(0.0f, 0.0f, 0.0f)
		, m_LookDir(0, 0, -1), m_RightDir(1, 0, 0), m_UpDir(0, 1, 0)
		, m_ProjMatrix(1.0f), m_ViewMatrix(1.0f), m_ViewProjMat(1.0f)
	{
	}

	void Camera3D::Move(const glm::vec3& delta)
	{
		m_Position += delta;

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookDir, m_UpDir);
		RecalcViewProj();
	}

	void Camera3D::SetPos(const glm::vec3& position)
	{
		m_Position = position;

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookDir, m_UpDir);
		RecalcViewProj();
	}

	void Camera3D::Rotate(Radians dPitch, Radians dYaw)
	{
		glm::vec3 dirDelta = {
			glm::cos(dYaw) * glm::cos(dPitch),
			glm::sin(dPitch),
			glm::sin(dYaw) * glm::cos(dPitch)
		};

		m_LookDir += dirDelta;
		m_LookDir = glm::normalize(m_LookDir);
		m_RightDir = glm::cross(m_LookDir, m_UpDir);

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookDir, m_UpDir);
		RecalcViewProj();
	}

	void Camera3D::SetRotation(Radians pitch, Radians yaw)
	{
		m_LookDir = {
			glm::cos(yaw) * glm::cos(pitch),
			glm::sin(pitch),
			glm::sin(yaw) * glm::cos(pitch)
		};

		m_LookDir = glm::normalize(m_LookDir);
		m_RightDir = glm::cross(m_LookDir, m_UpDir);

		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_LookDir, m_UpDir);
		RecalcViewProj();
	}

	void Camera3D::SetProjection(Radians fov, float aspect)
	{
		m_ProjMatrix = glm::perspective(fov, aspect, 0.1f, 100.0f);
		RecalcViewProj();
	}

	void Camera3D::RecalcViewProj()
	{
		m_ViewProjMat = m_ProjMatrix * m_ViewMatrix;
	}
}