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

        inline int      GetKeyCode() const { return m_Keycode; }
        inline int      getAction() const { return m_Action; }
        std::type_index GetEventType() const override { return typeid(KeyPressEvent); }

    private:
        int m_Keycode;
        int m_Action;
};

} // namespace Core

} // namespace Moonstone

#endif // INPUTEVENTS_H
