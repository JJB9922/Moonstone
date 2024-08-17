#include "Include/Logger.h"

namespace Moonstone
{
namespace Core
{

Logger::Logger() { spdlog::info("logger initialised"); }

Logger* CreateLoggerInstance() { return new Logger(); }

} // namespace Core
} // namespace Moonstone
