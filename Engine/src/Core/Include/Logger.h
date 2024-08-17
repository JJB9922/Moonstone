#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"

namespace Moonstone
{
namespace Core
{

class Logger
{
    public:
        Logger();
        ~Logger() = default;

        inline static Logger& GetLoggerInstance() { return *s_Logger; }

    private:
        static Logger* s_Logger;
};

Logger* CreateLoggerInstance();

} // namespace Core
} // namespace Moonstone

#endif // LOGGER_H
