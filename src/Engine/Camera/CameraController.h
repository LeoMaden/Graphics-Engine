#pragma once
#include "Camera.h"

namespace Engine {

	class CameraController
	{
	public:
		virtual void OnUpdate(float timestep) = 0;

		virtual const Camera& GetCamera() const = 0;
	};

}