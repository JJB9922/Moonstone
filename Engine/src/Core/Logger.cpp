#include "Core/Include/Logger.h"
#include <iostream>

namespace Moonstone
{
namespace Core
{

std::shared_ptr<spdlog::logger> Logger::s_Logger;

/**
 * @brief Initializes the logging system.
 *
 * Sets up the logger instance and prepares the logging system
 * for use. It must be called before any logging happens.
 */
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

    s_Logger = spdlog::stdout_color_mt("MOONSTONE");
    s_Logger->set_level(spdlog::level::trace);
}

/**
 * @brief Gets the logger instance.
 *
 * Provides access to the shared logger instance.
 *
 * @return std::shared_ptr<spdlog::logger>& A reference to the shared logger instance.
 */
Logger* CreateLoggerInstance() { return new Logger(); }

} // namespace Core
} // namespace Moonstone
