#include "mspch.h"

#include "Core/Include/Application.h"

int main()
{
    auto logger = Moonstone::Core::CreateLoggerInstance();
    auto app = Moonstone::Core::CreateApplicationInstance();

    app->Run();

    delete app;
    delete logger;
}
