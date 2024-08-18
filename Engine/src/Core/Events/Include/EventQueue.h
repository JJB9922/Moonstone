#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "Core/Events/Include/EventDispatcher.h"

namespace Moonstone
{

namespace Core
{

class EventQueue
{
    public:
        EventQueue(EventDispatcher& dispatcher)
            : m_Dispatcher(dispatcher)
        {
        }

        void enqueue(std::shared_ptr<Event> event) { m_EventQueue.push(std::move(event)); }
        void process()
        {
            while (!m_EventQueue.empty())
            {
                auto event = m_EventQueue.front();
                m_EventQueue.pop();
                m_Dispatcher.Dispatch(event);
            }
        }

    private:
        std::queue<std::shared_ptr<Event>> m_EventQueue;
        EventDispatcher&                   m_Dispatcher;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENTQUEUE_H
