#include "Utils.h"
#include "Log.h"

#include <fstream>
#include <sstream>

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
			buffer << line;
		}
		file.close();

		return buffer.str();
	}

}