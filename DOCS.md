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

# Events

Moonstone has a custom, queue based event system. The event system is set as a simple global singleton, and this hasn't yet caused issues - but may be refactored in future.

There are a few steps to setting up an event:

##### 1. Create the event class

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
{
	public:
		KeyPressEvent(int keycode, int action)
			: m_Keycode(keycode),
			  m_Action(action)
			  {}

		inline int GetKeyCode() const { return m_Keycode; }
		inline int GetAction() const { return m_Action; }
		inline std::type_index GetEventType const override { return typeid(KeyPressEvent); }

	private:
		int m_Keycode;
		int m_Action;
};
```

This concludes setup of the Event.

##### 2. Prepare the event dispatcher & queue instances

When adding an event to a new class, e.g. `Window`, we will need to get a couple of instances related to the event system. This is because the event system is currently treated as a single global system that handles all of the events within the application, so we need to hook into the global instance if we wish to perform operations with it.

``` Window.cpp

// Namespace Declarations

Moonstone::Core::EventDispatcher &eventDispatcher = Moonstone::Core::EventDispatcher::GetInstance();

Moonstone::Core::EventQueue &eventQueue = Moonstone::Core::EventQueue::GetInstance();

```

Now, the event dispatcher and queue are exposed to our class.

##### 3. Processing our event queue

Despite not having added anything to the event queue yet, it's worth making sure our event queue is being processed. In the example of a key press, we want to process any key inputs once per frame - which is every time that our window polls any events happening.

Also note that in this setup, our GLFW window is associated with our event queue so that it can access and modify things within the event queue from within GLFW's event callbacks. This makes sure that events detected by GLFW (such as a key press) can be directly added to the queue to be processed.

In the case of `Window.cpp`, I have a simple `while(window_should_be_open)` style loop that polls events every frame.

```Window.cpp

while(!glfwWindowShouldClose(m_Window)) 
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();

	eventQueue.process();
}

```

I have added the `eventQueue.process()` function call, which will essentially execute whatever is in the event queue each frame.

##### 4. Register a handler to the event

Before we can produce an event, we should make sure it actually does something. To do this, we will register an event handler to an event.

This just means we associate a function with an event - so when that event is called for processing, the system knows how to handle it. We can either do this by making a separate function for the implementation of our event, or by using a lambda function to keep everything in the same place. The basic format for a subscription is:

`eventDispatcher.Subscribe(EventType, EventHandlerFunction);`

These subscriptions can happen within the initialisation loop of a class.

```Window.cpp

void Window::SetupInitEvents() 
{

	eventDispatcher.Subscribe(typeid(KeyPressEvent), 
							[](std::shared_ptr<Event> event)
				{
					auto keyEvent = std::static_pointer_cast<KeyPressEvent>(event);
					//int key = keyEvent->GetKeyCode();
					int action = keyEvent->GetAction();

					switch (action)
					{
						case GLFW_PRESS:
							MS_DEBUG("key pressed");
							break;
						case GLFW_RELEASE;
							MS_DEBUG("key released");
							break;
					}
			
				});

}

```

Get the event, get any data associated that you need for processing the event, and process it how you see fit.

It's also a good opportunity here to track which events are subscribed. I've used a vector that stores the type information, so that I can unsubscribe from everything when the current class is terminated.

`m_SubscribedWindowEvents.push_back(typeid(MouseButtonPressEvent));`

##### 5. Produce and enqueue the event

Now that our event is associated to a handler, we will produce the event and add it to the queue. 

In this case, a key press is detected by GLFW, and this triggers our event to be produced and added to the queue, ready for processing. However, you can have this happen any time any specific event needs to be triggered. GLFW helpfully scans for key presses during `glfwPollEvents();` and will notify us of any detected presses, as well as allowing us to assign a callback function to be called when these presses are detected using `glfwSetKeyCallback`. Again, I'll stick with a lambda expression for brevity.

These callbacks can also be set up during the initialisation loop of the class.

In this callback handler, I will produce the event, retrieve the event queue that is associated with the GLFW window, then add the event to the queue ready to be processed when `eventQueue.process()` is called.

```Window.cpp

void Window::SetupInputCallbacks(GLFWwindow *window) 
{

	glfwSetKeyCallback(m_Window,
					[](GLFWwindow *window, int key, int scancode, int action, int mods){
				auto event = std::make_shared<KeyPressEvent>(key, action);
				auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
				eventQueue->enqueue(event);	
				});

}

```

##### 7. Unsubscribe

This is less important when the events are linked to the entire application's life cycle, but for smaller event systems it is important to unsubscribe when the class is destroyed.

Earlier, I mentioned adding subscribed events to an array. That leaves two ways to unsubscribe from events:
 
``` Window.cpp

void Window::TerminateWindow(){

for (auto &event : m_SubscribedWindowEvents)
    {
        eventDispatcher.Unsubscribe(event);
    }

...

}

```

Alternatively, this can be done manually with `eventDispatcher.Unsubscribe(typeid(KeyPressEvent));`

At the moment, the event bus just unsubscribes anything and everything with the provided type id - this is fine for now, but might need to be changed later if more granular event management is required.
