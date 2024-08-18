# Docs

# Logging

Logs come in 6 levels:

```
MS_TRACE()
MS_INFO()
MS_WARN()
MS_ERROR()
MS_DEBUG()
MS_LOUD_DEBUG()
```

Logging is as simple as calling a level with a string message as an argument:

`#include "Core/Include/Logger.h"`
`MS_INFO("Hello World!")`

Each of the log levels are quite self explanatory, except for the final one - `MS_LOUD_DEBUG()`.
This is essentially for anything that might fill the log quickly, e.g. a log that reports the mouse position on the screen every time it moves.

There are currently 2 defines within `Core/Include/Logger.h` that will determine the log level:

`#define MS_ENABLE_DEBUG_LOGGING`
`#define MS_ENABLE_LOUD_DEBUG_LOGGING`

Both of these must be enabled in order to see both `MS_DEBUG()` and `MS_LOUD_DEBUG()` logs.

​￼# Events

Moonstone has a custom, queue based event system. The event system is set as a simple global singleton, and this hasn't yet caused issues - but may be refactored in future.

There are a few steps to setting up an event:

​￼##### 1. Create the event class

The first thing we need when adding an event is the event itself.

Within `Core/Events/Include/`, we should either pick from an existing event category file or make a new one depending on the type of event we are adding. For example, a `KeyPressEvent` should go into the existing `InputEvents.h` file. Whereas an `PlaySoundEvent` should go into an `AudioEvents.h` file and so on.

Running with the example of a `KeyPressEvent`, I'll begin by creating the event in `InputEvents.h`.

``` InputEvents.h
// Header Guards

#include "Core/Events/Include/Event.h"

namespace Moonstone { namespace Core {

class KeyPressEvent : public Event {};

}}

// End Header Guards
```

A couple of important notes - we place events within the `Moonstone::Core::` namespace, and each event inherits from the `Event` base class.

Next, we set up the event appropriately. In this case, there are 2 key pieces of data I need. Those are the `keycode` of the key that was pressed, and the `action` that was taken (Pressed, Released, Held). I also need a way to report the event type.

``` InputEvents.h
class KeyPressEvent : public Event 
​￼{
	​￼public:
		​￼KeyPressEvent(int keycode, int action)
			​￼: m_Keycode(keycode),
			  m_Action(action)
			  {}

		inline int GetKeyCode() const { return m_Keycode; }
		inline int GetAction() const { return m_Action; }
		inline std::type_index GetEventType const override { return typeid(KeyPressEvent); }
```
