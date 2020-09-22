#include "CameraController3rdPerson.h"

namespace Engine {

	void CameraController3rdPerson::OnUpdate(float timestep)
	{
		//bool wDown = m_KeyDownFunc(Engine::KeyCode::W);
		//bool aDown = m_KeyDownFunc(Engine::KeyCode::A);
		//bool sDown = m_KeyDownFunc(Engine::KeyCode::S);
		//bool dDown = m_KeyDownFunc(Engine::KeyCode::D);
		//bool spaceDown = m_KeyDownFunc(KeyCode::Spacebar);
		//bool shiftDown = m_KeyDownFunc(KeyCode::LShift);

		//glm::vec3 moveDir(0.0f);

		//if (wDown)		moveDir += m_Camera.GetLookDir();
		//if (sDown)		moveDir -= m_Camera.GetLookDir();
		//if (dDown)		moveDir += m_Camera.GetRightDir();
		//if (aDown)		moveDir -= m_Camera.GetRightDir();
		//if (spaceDown)	moveDir += m_Camera.GetUpDir();
		//if (shiftDown)	moveDir -= m_Camera.GetUpDir();

		//if (moveDir == glm::vec3(0.0f))
		//{
		//	return;
		//}

		//moveDir = glm::normalize(moveDir) * m_Speed * timestep;

		//m_Camera.Move(moveDir);


	}

	void CameraController3rdPerson::SetCamera(const Camera3D& camera)
	{
		m_Camera = camera;
		UpdateProjection();
	}

	void CameraController3rdPerson::SetTarget(const glm::vec3& target)
	{
		m_Target = target;
		MoveAndLook();
	}

	void CameraController3rdPerson::SetTargetDistance(float dist)
	{
		m_TargetDistance = dist;
		MoveAndLook();
	}

	void CameraController3rdPerson::OnWindowResize(WindowResizeEvent& e)
	{
		int w = e.GetWidth();
		int h = e.GetHeight();

		m_Aspect = (float)w / (float)h;
		UpdateProjection();
	}

	void CameraController3rdPerson::OnMouseScroll(MouseScrollEvent& e)
	{
		m_Fov -= e.GetDistance() * m_ScrollSpeed;

		if (m_Fov < 30) m_Fov = 30;
		if (m_Fov > 100) m_Fov = 100;

		UpdateProjection();
	}

	void CameraController3rdPerson::OnMouseMove(MouseMoveEvent& e)
	{
		if (m_FirstLook)
		{
			m_LastMousePos = e.GetPosition();
			m_FirstLook = false;
		}

		// Left mouse must be down to rotate camera.
		if (e.GetModifier().Mouse1 == false)
		{
			m_LastMousePos = e.GetPosition();
			return;
		}

		glm::vec2 mouseDelta = e.GetPosition() - m_LastMousePos;
		m_LastMousePos = e.GetPosition();

		m_AngleAround += -mouseDelta.x * m_LookSensitivity;
		m_AngleElevation += -mouseDelta.y * m_LookSensitivity;

		if (m_AngleElevation < 1.0f) m_AngleElevation = 1.0f;
		if (m_AngleElevation > 179.0f) m_AngleElevation = 179.0f;

		MoveAndLook();
	}

	void CameraController3rdPerson::UpdateProjection()
	{
		m_Camera.SetProjection(glm::radians(m_Fov), m_Aspect);
	}

	void CameraController3rdPerson::MoveAndLook()
	{
		Radians theta = glm::radians(m_AngleElevation), phi = glm::radians(m_AngleAround);

		float camX = m_TargetDistance * glm::sin(theta) * glm::sin(phi);
		float camY = m_TargetDistance * glm::cos(theta);
		float camZ = m_TargetDistance * glm::sin(theta) * glm::cos(phi);

		glm::vec3 camUp = glm::normalize(glm::vec3(-camX, -camY, glm::sin(theta)));

		m_Camera.SetPos(glm::vec3(camX, camY, camZ));
		m_Camera.LookAt(m_Target, glm::vec3(0, 1, 0));
	}

}