#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "Core/Events/Include/EventManager.h"
#include "Core/Events/Include/IEventListener.h"

namespace Moonstone
{

namespace Core
{

class EventListener : public IEventListener
{
    public:
        EventListener(EventManager& eventManager)
            : m_EventManager(eventManager)
        {
            this->m_EventManager.AddListener(this);
        }

        virtual ~EventListener() = default;

        void onEvent(std::shared_ptr<Event> event) override
        {
            m_Event = event;
            LogOnEvent();
        }

        void DetachEvent() { m_EventManager.RemoveListener(this); }

        void LogOnEvent()
        {
            std::stringstream ss;
            ss << "event triggered: " << m_Event->GetEventID();
            MS_INFO(ss.str());
        }

    private:
        std::shared_ptr<Event> m_Event;
        EventManager& m_EventManager;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENTLISTENER_H
