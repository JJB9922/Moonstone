#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H

#include "Core/Events/Include/Event.h"

namespace Moonstone
{

namespace Core
{

class WindowResizeEvent : public Event
{
    public:
        WindowResizeEvent(const std::string eventID, unsigned width, unsigned height)
            : m_Width(width)
            , m_Height(height)
            , m_EventID(eventID)
        {
        }

        inline unsigned    GetWidth() const { return m_Width; }
        inline unsigned    GetHeight() const { return m_Height; }
        inline std::string GetEventID() const { return m_EventID; }

    private:
        unsigned    m_Width, m_Height;
        std::string m_EventID;
};

class WindowCloseEvent : public Event
{
    public:
        WindowCloseEvent(const std::string eventID)
            : m_EventID(eventID)
        {
        }

        inline std::string GetEventID() const { return m_EventID; }

    private:
        std::string m_EventID;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOWEVENT_H
