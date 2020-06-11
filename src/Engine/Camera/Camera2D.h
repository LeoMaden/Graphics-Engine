#pragma once
#include "Camera.h"

namespace Engine {

	class Camera2D : public Camera
	{
	public:
		Camera2D();

		void Move(const glm::vec3& delta);
		void SetPos(const glm::vec3& position);

		virtual const glm::mat4& GetViewProjMat() const override { return m_ViewProjMat; }

		void SetProjection(const glm::vec2& size, const glm::vec2& centre);

	private:
		glm::vec3 m_Position;

		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjMat;

		void RecalcViewProj();
	};

}