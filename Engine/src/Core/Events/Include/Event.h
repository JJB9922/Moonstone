#ifndef EVENT_H
#define EVENT_H

#include "Core/Include/Core.h"

/**
 *
 * Event system built using the Observer design pattern
 * 
 * Event Instances = Event
 * Observer = Subscriber = EventListener
 * Subject = Publisher = EventManager
 *
 */

namespace Moonstone
{

namespace Core
{

class Event
{
    public:
        Event(const std::string eventID)
            : m_EventID(eventID)
        {
        }

        inline virtual std::string GetEventID() const { return m_EventID; }

    private:
        std::string m_EventID;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENT_H
