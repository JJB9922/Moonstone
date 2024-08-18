#ifndef LOGGER_H
#define LOGGER_H

#include "Core/Include/Core.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

namespace Moonstone
{
namespace Core
{

class Logger
{
    public:
        Logger()  = default;
        ~Logger() = default;

        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetLoggerInstance() { return s_Logger; }

    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
};

Logger* CreateLoggerInstance();

} // namespace Core
} // namespace Moonstone

#define MS_TRACE(...) ::Moonstone::Core::Logger::GetLoggerInstance()->trace(__VA_ARGS__)
#define MS_INFO(...) ::Moonstone::Core::Logger::GetLoggerInstance()->info(__VA_ARGS__)
#define MS_WARN(...) ::Moonstone::Core::Logger::GetLoggerInstance()->warn(__VA_ARGS__)
#define MS_ERROR(...) ::Moonstone::Core::Logger::GetLoggerInstance()->error(__VA_ARGS__)

#define MS_ENABLE_DEBUG_LOGGING
//#define MS_ENABLE_LOUD_DEBUG_LOGGING

#ifdef MS_ENABLE_DEBUG_LOGGING
#define MS_DEBUG(...) ::Moonstone::Core::Logger::GetLoggerInstance()->debug(__VA_ARGS__)
#else
#define MS_DEBUG(...)
#define MS_LOUD_DEBUG(...)
#endif

#ifdef MS_ENABLE_LOUD_DEBUG_LOGGING
#define MS_LOUD_DEBUG(...) ::Moonstone::Core::Logger::GetLoggerInstance()->debug(__VA_ARGS__)
#else
#define MS_LOUD_DEBUG(...)
#endif

#endif // LOGGER_H
