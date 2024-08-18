#ifndef WINDOWEVENTS_H
#define WINDOWEVENTS_H

#include "Core/Events/Include/Event.h"

namespace Moonstone
{

namespace Core
{

class WindowCloseEvent : public Event
{
    public:
        WindowCloseEvent() {}

        inline std::type_index GetEventType() const override { return typeid(WindowCloseEvent); }
};

class WindowResizeEvent : public Event
{
    public:
        WindowResizeEvent(int width, int height)
            : m_Width(width)
            , m_Height(height)
        {
        }

        inline int      GetWidth() const { return m_Width; }
        inline int      GetHeight() const { return m_Height; }
        inline std::type_index GetEventType() const override { return typeid(WindowResizeEvent); }

    private:
        int m_Width, m_Height;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOWEVENTS_H
