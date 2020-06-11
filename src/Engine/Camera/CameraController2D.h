#pragma once
#include "Camera2D.h"
#include "CameraController.h"

#include "Events/KeyCodes.h"

#include <functional>

namespace Engine {

	class CameraController2D : public CameraController
	{
		using IsKeyDownFunc = std::function<bool(KeyCode)>;
	public:
		void SetKeyDownFunc(IsKeyDownFunc keyDownFunc) { m_KeyDownFunc = keyDownFunc; }

		virtual void OnUpdate(float timestep) override;

		virtual const Camera& GetCamera() const override	{ return m_Camera; }
		void SetCamera(const Camera2D& camera)				{ m_Camera = camera; }

	private:
		Camera2D			m_Camera;
		IsKeyDownFunc		m_KeyDownFunc;
		float				m_Speed = 0.1f;
	};

}