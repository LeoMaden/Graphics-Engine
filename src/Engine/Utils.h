#pragma once
#include <string>

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
	};

}