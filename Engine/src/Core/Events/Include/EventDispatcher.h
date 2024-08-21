#ifndef EVENTDISPATCHER_H
#define EVENTDISPATCHER_H

#include "Core/Events/Include/Event.h"
#include "Core/Include/Logger.h"

namespace Moonstone
{

namespace Core
{

/**
 * @brief Manages event subscriptions and dispatches events to subscribed callbacks.
 * 
 * The `EventDispatcher` class follows the singleton pattern to ensure a single instance is used throughout the application.
 * It maintains a map of event types to their corresponding callbacks. Events are dispatched to all callbacks subscribed
 * to their type. The class supports subscribing to, unsubscribing from, and dispatching events.
 */
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
            MS_DEBUG("subscribing {0}", eventType.name());
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
