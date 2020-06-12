#include "Utils.h"
#include "Log.h"

#include <fstream>
#include <sstream>
#undef LoadImage

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Engine {

	std::string Utils::LoadFile(const std::string& path)
	{
		std::stringstream buffer;
		std::string line;

		std::fstream file;
		file.open(path);

		ASSERT(file, "Could not open file {}", path);

		while (getline(file, line))
		{
			buffer << line << "\n";
		}
		file.close();

		return buffer.str();
	}

	uint8_t* Utils::LoadImage(const std::string& path, ImageProps* props)
	{
		stbi_set_flip_vertically_on_load(true);
		uint8_t* buf = stbi_load(path.c_str(), &props->Width, &props->Height, &props->Channels, 0);

		ASSERT(buf, "Could not open image");
		if (!buf) return nullptr;

		return buf;
	}

	void Utils::FreeImage(uint8_t* data)
	{
		stbi_image_free(data);
	}

}