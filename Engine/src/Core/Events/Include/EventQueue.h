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
        EventQueue() { m_Dispatcher = EventDispatcher::GetEventDispatcherInstance(); }
        ~EventQueue() = default;

        static void Init();

        inline static std::shared_ptr<EventQueue>& GetEventQueueInstance()
        {
            MS_ASSERT(s_EventQueue, "event queue failed to initialise");
            return s_EventQueue;
        }

        void Enqueue(std::shared_ptr<Event> event) { m_Queue.push(std::move(event)); }

        void Process()
        {
            while (!m_Queue.empty())
            {
                auto event = m_Queue.front();
                m_Queue.pop();
                m_Dispatcher->Dispatch(event);
            }
        }

    private:
        static std::shared_ptr<EventQueue> s_EventQueue;
        std::queue<std::shared_ptr<Event>> m_Queue;
        std::shared_ptr<EventDispatcher>   m_Dispatcher;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENTQUEUE_H
