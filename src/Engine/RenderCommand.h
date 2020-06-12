#pragma once

namespace Engine {

	class RenderCommand
	{
	public:
		static void Clear();

		static void EnableBlending(bool setting = true);
	};

}