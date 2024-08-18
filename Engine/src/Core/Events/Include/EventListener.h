#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "Core/Events/Include/EventManager.h"
#include "Core/Events/Include/IEventListener.h"

namespace Moonstone
{

namespace Core
{

template<typename EventType>
class EventListener : public IEventListener
{
    public:
        EventListener(EventManager& eventManager, std::shared_ptr<EventType> event)
            : m_EventManager(eventManager)
        {
            this->m_Event = event;
            this->m_EventManager.AddListener(this);
        }

        virtual ~EventListener() = default;

        void onEvent() override
        {
            LogOnEvent();
            m_Event->Handled(true);
        }

        void DetachEvent()
        {
            m_Event = nullptr;
            m_EventManager.RemoveListener(this);
        }

        void LogOnEvent()
        {
            std::string       eventID = m_Event->GetEventID();
            std::stringstream ss;
            ss << "event triggered: " << eventID;
            MS_INFO(ss.str());
        }

    private:
        std::shared_ptr<EventType> m_Event;
        EventManager& m_EventManager;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENTLISTENER_H
