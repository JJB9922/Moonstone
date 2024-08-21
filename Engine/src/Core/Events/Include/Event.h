#ifndef EVENT_H
#define EVENT_H

#include "Core/Include/Core.h"

namespace Moonstone
{

namespace Core
{

/**
 * @brief Base class for all events in the system.
 * 
 * The `Event` class provides a common interface for events, allowing them to be identified and handled uniformly.
 * It includes a pure virtual method to obtain the event type and a static method to hash event names.
 */
class Event
{
    public:
        virtual ~Event() = default;

        virtual std::type_index GetEventType() const = 0;

        static std::size_t HashId(const std::string& name) { return std::hash<std::string>{}(name); }
};

} // namespace Core

} // namespace Moonstone

#endif // EVENT_H
