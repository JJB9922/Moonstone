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
        inline virtual std::string GetEventID() const { return m_EventID; }
        inline virtual void        Handled(bool handled) { m_Handled = handled; }
        inline virtual bool        IsHandled() const { return m_Handled; }

    private:
        std::string m_EventID;
        bool        m_Handled;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENT_H
