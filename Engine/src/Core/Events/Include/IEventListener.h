#ifndef IEVENTLISTENER_H
#define IEVENTLISTENER_H

#include "Core/Events/Include/Event.h"
#include "Core/Include/Core.h"

namespace Moonstone
{

namespace Core
{

class IEventListener
{
    public:
        virtual ~IEventListener()                = default;
        virtual void onEvent(std::shared_ptr<Event> event) = 0;
};

} // namespace Core

} // namespace Moonstone

#endif // IEVENTLISTENER_H
