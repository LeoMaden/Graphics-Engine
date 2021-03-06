#pragma once
#include "Camera.h"

namespace Engine {

	using Degrees = float;
	using Radians = float;

	class Camera3D : public Camera
	{
	public:
		Camera3D();

		void Move(const glm::vec3& delta);
		void SetPos(const glm::vec3& position);

		void Rotate(Radians dPitch, Radians dYaw);
		void SetRotation(Radians pitch, Radians yaw);
		void LookAt(const glm::vec3& target, const glm::vec3& upDir);

		void SetClipPlanes(float zNear, float zFar);

		virtual const glm::mat4& GetViewProjMat() const override { return m_ViewProjMat; }
		virtual const glm::vec3& GetPos() const override { return m_Position; }

		const glm::vec3& GetLookDir()	const { return m_LookDir; }
		const glm::vec3& GetRightDir()	const { return m_RightDir; }
		const glm::vec3& GetUpDir()		const { return m_UpDir; }

		void SetProjection(Radians fov, float aspect);

	private:
		glm::vec3 m_Position;

		glm::vec3 m_LookDir;
		glm::vec3 m_RightDir;
		glm::vec3 m_UpDir;
		float		m_Pitch = 0.0f;
		float		m_Yaw = 0.0f;

		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjMat;

		float m_NearClip;
		float m_FarClip;

		void RecalcViewProj();
	};

}