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

}