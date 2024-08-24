#include "Core/Events/Include/EventDispatcher.h"

namespace Moonstone
{
namespace Core
{

std::shared_ptr<EventDispatcher> EventDispatcher::s_EventDispatcher;

void EventDispatcher::Init()
{
    s_EventDispatcher = std::make_shared<EventDispatcher>();
    MS_INFO("event dispatcher initialised");
}

} // namespace Core
} // namespace Moonstone
