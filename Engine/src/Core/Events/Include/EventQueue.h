#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#include "Core/Events/Include/EventDispatcher.h"

namespace Moonstone
{

namespace Core
{

/**
 * @brief Manages a queue of events, providing functionality to enqueue and process events.
 * 
 * The `EventQueue` class is a singleton that holds and processes a queue of events using an `EventDispatcher`.
 * Events are enqueued and then dispatched in the `process` method. The singleton pattern ensures that only one
 * instance of the `EventQueue` exists and is accessible throughout the application.
 */
class EventQueue
{
    public:
        static EventQueue& GetInstance()
        {
            static EventQueue instance(GetInstanceDispatcher());
            return instance;
        };

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
        EventQueue(EventDispatcher& dispatcher)
            : m_Dispatcher(dispatcher)
        {
        }
        EventQueue(const EventQueue&)            = delete;
        EventQueue& operator=(const EventQueue&) = delete;

        static EventDispatcher& GetInstanceDispatcher() { return EventDispatcher::GetInstance(); }

        std::queue<std::shared_ptr<Event>> m_EventQueue;
        EventDispatcher&                   m_Dispatcher;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENTQUEUE_H
