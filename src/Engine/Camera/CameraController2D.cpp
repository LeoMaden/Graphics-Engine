#include "Pch.h"
#include "CameraController2D.h"

namespace Engine {

	void CameraController2D::OnUpdate(float timestep)
	{
		bool wDown = m_KeyDownFunc(Engine::KeyCode::W);
		bool aDown = m_KeyDownFunc(Engine::KeyCode::A);
		bool sDown = m_KeyDownFunc(Engine::KeyCode::S);
		bool dDown = m_KeyDownFunc(Engine::KeyCode::D);

		glm::vec2 moveDir(0.0f);

		if (wDown)		moveDir += glm::vec2(0, -1);
		if (aDown)		moveDir += glm::vec2(1,  0);
		if (sDown)		moveDir += glm::vec2(0,  1);
		if (dDown)		moveDir += glm::vec2(-1, 0);

		if (moveDir == glm::vec2(0.0f))
		{
			return;
		}

		moveDir = glm::normalize(moveDir) * m_Speed * timestep;

		m_Camera.Move(glm::vec3(moveDir, 0.0f));
	}

	void CameraController2D::SetCamera(const Camera2D& camera)
	{
		m_Camera = camera;
		UpdateProjection();
	}

	void CameraController2D::OnWindowResize(WindowResizeEvent& e)
	{
		int w = e.GetWidth();
		int h = e.GetHeight();

		m_Aspect = (float)w / (float)h;
		UpdateProjection();
	}

	void CameraController2D::OnMouseScroll(MouseScrollEvent& e)
	{
		m_Zoom -= e.GetDistance() * m_ScrollSpeed;

		if (m_Zoom < 2) m_Zoom = 2;
		if (m_Zoom > 10) m_Zoom = 10;

		UpdateProjection();
	}

	void CameraController2D::UpdateProjection()
	{
		glm::vec2 size = { m_Zoom * m_Aspect, m_Zoom };
		glm::vec2 centre = size / 2.0f;

		m_Camera.SetProjection(size, centre);
	}

}