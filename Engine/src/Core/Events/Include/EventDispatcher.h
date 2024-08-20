#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "Core/Events/Include/Event.h"
#include "Core/Include/Logger.h"

namespace Moonstone
{

namespace Core
{

class EventDispatcher
{
    public:
        static EventDispatcher& GetInstance()
        {
            static EventDispatcher instance;
            return instance;
        }

        using Callback = std::function<void(std::shared_ptr<Event>)>;

        void Subscribe(const std::type_index& eventType, Callback callback)
        {
            m_Subscribers[eventType].push_back(std::move(callback));
        }

        void Unsubscribe(const std::type_index& eventType)
        {
            MS_DEBUG("unsubscribing {0}", eventType.name());
            m_Subscribers.erase(eventType);
        }

        void Dispatch(std::shared_ptr<Event> event) const
        {
            auto it = m_Subscribers.find(event->GetEventType());

            if (it != m_Subscribers.end())
            {
                for (const auto& callback : it->second)
                {
                    callback(event);
                }
            }
        }

    private:
        EventDispatcher();
        EventDispatcher(const EventDispatcher&)            = delete;
        EventDispatcher& operator=(const EventDispatcher&) = delete;

    private:
        std::unordered_map<std::type_index, std::vector<Callback>> m_Subscribers;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENTDISPATCHER_H
