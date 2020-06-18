#include "CameraControllerFPS.h"

namespace Engine {

	void CameraControllerFPS::OnUpdate(float timestep)
	{
		bool wDown = m_KeyDownFunc(Engine::KeyCode::W);
		bool aDown = m_KeyDownFunc(Engine::KeyCode::A);
		bool sDown = m_KeyDownFunc(Engine::KeyCode::S);
		bool dDown = m_KeyDownFunc(Engine::KeyCode::D);
		bool spaceDown = m_KeyDownFunc(KeyCode::Spacebar);
		bool shiftDown = m_KeyDownFunc(KeyCode::LShift);

		glm::vec3 moveDir(0.0f);

		if (wDown)		moveDir += glm::vec3(0, 0, 1);
		if (aDown)		moveDir += glm::vec3(1, 0, 0);
		if (sDown)		moveDir += glm::vec3(0, 0, -1);
		if (dDown)		moveDir += glm::vec3(-1, 0, 0);
		if (spaceDown)	moveDir += glm::vec3(0, -1, 0);
		if (shiftDown)	moveDir += glm::vec3(0, 1, 0);

		if (moveDir == glm::vec3(0.0f))
		{
			return;
		}

		moveDir = glm::normalize(moveDir) * m_Speed * timestep;

		m_Camera.Move(moveDir);
	}

	void CameraControllerFPS::SetCamera(const Camera3D& camera)
	{
		m_Camera = camera;
		UpdateProjection();
	}

	void CameraControllerFPS::OnWindowResize(WindowResizeEvent& e)
	{
		int w = e.GetWidth();
		int h = e.GetHeight();

		m_Aspect = (float)w / (float)h;
		UpdateProjection();
	}

	void CameraControllerFPS::OnMouseScroll(MouseScrollEvent& e)
	{
		m_Fov -= e.GetDistance() * m_ScrollSpeed;

		if (m_Fov < 30) m_Fov = 30;
		if (m_Fov > 100) m_Fov = 100;

		UpdateProjection();
	}

	void CameraControllerFPS::UpdateProjection()
	{
		m_Camera.SetProjection(glm::radians(m_Fov), m_Aspect);
	}

}