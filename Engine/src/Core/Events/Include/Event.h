#ifndef EVENT_H
#define EVENT_H

#include "Core/Include/Core.h"

namespace Moonstone
{

namespace Core
{

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
