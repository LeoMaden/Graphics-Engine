#pragma once
#include "Camera2D.h"
#include "CameraController.h"

#include "Events/WindowEvents.h"
#include "Core/KeyCodes.h"
#include "Events/MouseEvents.h"

#include <functional>

namespace Engine {

	class CameraController2D : public CameraController
	{
		using IsKeyDownFunc = std::function<bool(KeyCode)>;
	public:
		void SetKeyDownFunc(IsKeyDownFunc keyDownFunc) { m_KeyDownFunc = keyDownFunc; }

		virtual void OnUpdate(float timestep) override;

		virtual const Camera& GetCamera() const override	{ return m_Camera; }
		void SetCamera(const Camera2D& camera);

		void OnWindowResize(WindowResizeEvent& e);
		void OnMouseScroll(MouseScrollEvent& e);

	private:
		void UpdateProjection();

		Camera2D			m_Camera;
		IsKeyDownFunc		m_KeyDownFunc;
		float				m_Speed = 8.0f;

		float m_Zoom	= 10.0f;
		float m_Aspect	= 16.0f / 9.0f;
		float m_ScrollSpeed = 2.0f;
	};

}