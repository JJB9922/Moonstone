#include "mspch.h"

#include "Core/Include/Application.h"

int main()
{
    Moonstone::Core::Logger::Init();
    MS_INFO("application started successfully");
    Moonstone::Core::EventDispatcher& eventDispatcher = Moonstone::Core::EventDispatcher::GetInstance();
    Moonstone::Core::EventQueue&      eventQueue      = Moonstone::Core::EventQueue::GetInstance();

    auto app = Moonstone::Core::CreateApplicationInstance();
    app->Run();

    delete app;
}
