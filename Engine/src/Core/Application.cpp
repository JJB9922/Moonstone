#include "Include/Application.h"
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

void Application::Run() { Window::CreateWindow(); }

Application* CreateApplicationInstance() { return new Application(); }

} // namespace Core
} // namespace Moonstone
