#include "Core/Include/Logger.h"
#include <iostream>

namespace Moonstone
{
namespace Core
{

std::shared_ptr<spdlog::logger> Logger::s_Logger;

void Logger::Init()
{
    try
    {
        auto file_logger = spdlog::rotating_logger_mt("moonstone_logger",
                                                      "logs/current.log",
                                                      1024 * 1024 * 5,
                                                      3);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "log initialisation failed: " << ex.what() << '\n';
    }

    s_Logger = spdlog::stdout_logger_mt("MOONSTONE");
    s_Logger->set_level(spdlog::level::trace);
}

Logger* CreateLoggerInstance() { return new Logger(); }

} // namespace Core
} // namespace Moonstone
