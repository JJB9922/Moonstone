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

class WindowMinimizeEvent : public Event
{
    public:
        WindowMinimizeEvent(int minimized)
            : m_Minimized(minimized)
        {
        }

        inline int             IsMinimized() const { return m_Minimized; }
        inline std::type_index GetEventType() const override { return typeid(WindowMinimizeEvent); }

    private:
        int m_Minimized;
};

class WindowFocusEvent : public Event
{
    public:
        WindowFocusEvent(int focused)
            : m_Focused(focused)
        {
        }

        inline int             IsFocused() const { return m_Focused; }
        inline std::type_index GetEventType() const override { return typeid(WindowFocusEvent); }

    private:
        int m_Focused;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOWEVENTS_H
