#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

#include "Core/Events/Include/Event.h"

namespace Moonstone
{

namespace Core
{

class IEventListener
{
    public:
        virtual ~IEventListener()                = default;
        virtual void onEvent()                   = 0;
};

} // namespace Core

} // namespace Moonstone

#endif // IEVENTLISTENER_H
