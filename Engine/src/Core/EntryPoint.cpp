#include "mspch.h"

#include "Core/Include/Application.h"

int main()
{
    Moonstone::Core::Logger::Init();

    Moonstone::Core::EventDispatcher &eventDispatcher = Moonstone::Core::EventDispatcher::GetInstance();
    Moonstone::Core::EventQueue      &eventQueue      = Moonstone::Core::EventQueue::GetInstance();

    MS_INFO("application started successfully");

    auto app = Moonstone::Core::CreateApplicationInstance();
    app->Run();

    delete app;
}
