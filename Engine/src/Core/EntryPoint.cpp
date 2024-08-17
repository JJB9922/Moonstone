#include "mspch.h"

#include "Core/Include/Application.h"

int main()
{
    Moonstone::Core::Logger::Init();
    MS_INFO("application started successfully");

    auto app = Moonstone::Core::CreateApplicationInstance();
    app->Run();

    delete app;
}
