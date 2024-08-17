#include "Include/Application.h"

#include "Core/Events/Include/EventListener.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

Application* Application::s_ApplicationInstance = nullptr;

Application::Application()
{
    if (s_ApplicationInstance)
    {
        MS_ASSERT(!s_ApplicationInstance, "application instance already exists");
    }

    s_ApplicationInstance = this;
}

void Application::Run()
{
    // Testing Event System

    std::unique_ptr<EventManager>  eventMgr      = std::make_unique<EventManager>();
    std::unique_ptr<EventListener> eventListener = std::make_unique<EventListener>(*eventMgr);
    std::shared_ptr<Event>         startupEvent  = std::make_shared<Event>("StartupEvent");

    eventListener->onEvent(startupEvent);
    eventMgr->Dispatch();
    eventListener->DetachEvent();

    // End Testing Event System

    m_Window = std::unique_ptr<Window>(Window::CreateWindow());
}

Application* CreateApplicationInstance() { return new Application(); }

} // namespace Core
} // namespace Moonstone
