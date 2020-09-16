#pragma once
#include <string>

#include "Scene/Scene.h"

#undef LoadImage

namespace Engine {

	/*static*/ class Utils
	{
	public:
		struct ImageProps
		{
			int Width;
			int Height;
			int Channels;
		};

		static std::string LoadFile(const std::string& path);

		static uint8_t* LoadImage(const std::string& path, ImageProps* props);
		static void FreeImage(uint8_t* data);

		static Scene* LoadScene(const std::string& path);
	};

}