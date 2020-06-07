#pragma once
#include <spdlog/spdlog.h>

namespace Engine {

	/*static*/ class Log
	{
	public:
		static std::shared_ptr<spdlog::logger> Logger;

		static void Init();
		static void SetLogLevel(spdlog::level::level_enum level);
	};

}

#define LOG_TRACE(...)		Engine::Log::Logger->trace(__VA_ARGS__)
#define LOG_DEBUG(...)		Engine::Log::Logger->debug(__VA_ARGS__)
#define LOG_INFO(...)		Engine::Log::Logger->info(__VA_ARGS__)
#define LOG_WARN(...)		Engine::Log::Logger->warn(__VA_ARGS__)
#define LOG_ERROR(...)		Engine::Log::Logger->error(__VA_ARGS__)
#define LOG_CRITICAL(...)	Engine::Log::Logger->critical(__VA_ARGS__)

#define BREAK __debugbreak()
#define ASSERT(x, ...)		if(!x) { LOG_ERROR(__VA_ARGS__); BREAK; }
