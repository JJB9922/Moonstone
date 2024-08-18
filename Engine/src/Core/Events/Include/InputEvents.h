#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H

#include "Core/Events/Include/Event.h"

namespace Moonstone
{

namespace Core
{

class KeyPressEvent : public Event
{
    public:
        KeyPressEvent(int keycode, int action)
            : m_Keycode(keycode)
            , m_Action(action)
        {
        }

        inline int             GetKeyCode() const { return m_Keycode; }
        inline int             GetAction() const { return m_Action; }
        inline std::type_index GetEventType() const override { return typeid(KeyPressEvent); }

    private:
        int m_Keycode;
        int m_Action;
};

class MouseButtonPressEvent : public Event
{
    public:
        MouseButtonPressEvent(int button, int action)
            : m_Button(button)
            , m_Action(action)
        {
        }

        inline int             GetButton() const { return m_Button; }
        inline int             GetAction() const { return m_Action; }
        inline std::type_index GetEventType() const override { return typeid(MouseButtonPressEvent); }

    private:
        int m_Button;
        int m_Action;
};

class MouseScrollEvent : public Event
{
    public:
        MouseScrollEvent(int xoffset, int yoffset)
            : m_XOffset(xoffset)
            , m_YOffset(yoffset)
        {
        }

        inline int             GetXOffset() const { return m_XOffset; }
        inline int             GetYOffset() const { return m_YOffset; }
        inline std::type_index GetEventType() const override { return typeid(MouseScrollEvent); }

    private:
        int m_XOffset, m_YOffset;
};

class MouseMoveEvent : public Event
{
    public:
        MouseMoveEvent(double xPosition, double yPosition)
            : m_XPosition(xPosition)
            , m_YPosition(yPosition)
        {
        }

        inline double          GetXPosition() const { return m_XPosition; }
        inline double          GetYPosition() const { return m_YPosition; }
        inline std::type_index GetEventType() const override { return typeid(MouseMoveEvent); }

    private:
        double m_XPosition, m_YPosition;
};

} // namespace Core

} // namespace Moonstone

#endif // INPUTEVENTS_H
