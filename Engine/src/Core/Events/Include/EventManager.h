#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "Core/Events/Include/IEventListener.h"
#include "Core/Include/Core.h"
#include "Core/Include/Logger.h"

namespace Moonstone
{

namespace Core
{

class IEventManager
{
    public:
        virtual ~IEventManager()                              = default;
        virtual void AddListener(IEventListener* listener)    = 0;
        virtual void RemoveListener(IEventListener* listener) = 0;
        virtual void Dispatch()                               = 0;
};

class EventManager : public IEventManager
{
    public:
        virtual ~EventManager() { MS_INFO("eventmanager destroyed"); }

        void AddListener(IEventListener* listener) override { m_Listeners.push_back(listener); }
        void RemoveListener(IEventListener* listener) override
        {
            m_Listeners.erase(std::remove(m_Listeners.begin(), m_Listeners.end(), listener),
                              m_Listeners.end());
        }

        void Dispatch() override
        {
            ReportNumberOfEventListeners();

            if (!m_Event)
            {
                MS_ERROR("no event/s found for the event manager to dispatch");
                return;
            }
            for (auto* listener : m_Listeners)
            {
                listener->onEvent(m_Event);
            }
        }

        void ReportNumberOfEventListeners()
        {
            std::stringstream ss;
            ss << "dispatcher reported " << m_Listeners.size() << " listeners";
            MS_INFO(ss.str());
        }

    private:
        std::vector<IEventListener*> m_Listeners;
        std::shared_ptr<Event>       m_Event;
};

} // namespace Core

} // namespace Moonstone

#endif // EVENTMANAGER_H
