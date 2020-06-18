#pragma once
#include "Camera.h"

namespace Engine {

	class Camera3D : public Camera
	{
	public:
		Camera3D();

		void Move(const glm::vec3& delta);
		void SetPos(const glm::vec3& position);

		virtual const glm::mat4& GetViewProjMat() const override { return m_ViewProjMat; }

		void SetProjection(float fov, float aspect);

	private:
		glm::vec3 m_Position;

		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjMat;

		void RecalcViewProj();
	};

}