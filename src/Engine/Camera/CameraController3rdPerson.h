#pragma once
#include "Camera3D.h"
#include "CameraController.h"

#include "Events/WindowEvents.h"
#include "Events/KeyCodes.h"
#include "Events/MouseEvents.h"

#include <functional>

namespace Engine {

	class CameraController3rdPerson : public CameraController
	{
		//using IsKeyDownFunc = std::function<bool(KeyCode)>;

	public:
		//void SetKeyDownFunc(IsKeyDownFunc keyDownFunc) { m_KeyDownFunc = keyDownFunc; }

		virtual void OnUpdate(float timestep) override;

		virtual const Camera& GetCamera() const override { return m_Camera; }
		void SetCamera(const Camera3D& camera);

		void SetTarget(const glm::vec3& target);
		void SetTargetDistance(float dist);

		void OnWindowResize(WindowResizeEvent& e);
		void OnMouseScroll(MouseScrollEvent& e);
		void OnMouseMove(MouseMoveEvent& e);

	private:
		void UpdateProjection();
		void MoveAndLook();

		Camera3D			m_Camera;
		float				m_Speed = 8.0f;
		//IsKeyDownFunc		m_KeyDownFunc;

		glm::vec3			m_Target;
		float				m_TargetDistance	= 10.0f;		// Distance from target ie sphere radius.
		Degrees				m_AngleAround		= 0.0f;		// Angle from x axis 
		Degrees				m_AngleElevation	= 45.0f;	// Angle from z axis

		Degrees		m_Fov = 60.0f;
		float		m_Aspect = 16.0f / 9.0f;
		float		m_ScrollSpeed = 2.0f;

		glm::vec2	m_LastMousePos = { 0, 0 };
		bool		m_FirstLook = true;
		float		m_LookSensitivity = 0.5f;
	};

}