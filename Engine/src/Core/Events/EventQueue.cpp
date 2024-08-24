#include "Core/Events/Include/EventQueue.h"

namespace Moonstone
{

namespace Core
{

std::shared_ptr<EventQueue> EventQueue::s_EventQueue;

void EventQueue::Init()
{
    s_EventQueue = std::make_shared<EventQueue>();
    MS_INFO("event queue initialised");
}

} // namespace Core

} // namespace Moonstone
