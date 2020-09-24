#include "Pch.h"
#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Engine {

	/*static*/ std::shared_ptr<spdlog::logger> Log::Logger;

	/*static*/ void Log::Init()
	{
		Logger = spdlog::stdout_color_mt("logger");

		Logger->set_pattern("%^(%T)%@ %v %$");
		Logger->set_level(spdlog::level::trace);
	}

	/*static*/ void Log::SetLogLevel(spdlog::level::level_enum level)
	{
		Logger->set_level(level);
	}

}