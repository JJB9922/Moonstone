#include "Core/Include/Application.h"

int main()
{
    Moonstone::Core::Logger::Init();
    Moonstone::Core::EventDispatcher::Init();
    Moonstone::Core::EventQueue::Init();

    MS_INFO("application initialised successfully");

    auto app = Moonstone::Core::CreateApplicationInstance();
    app->InitializeEditor();
    app->Run();
}
