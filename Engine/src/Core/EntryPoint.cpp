#include "mspch.h"

#include "Core/Include/Application.h"

/**
 * @brief The entry point of the Moonstone application.
 *
 * This function initializes the logging system, creates an application instance,
 * runs the application, and then cleans up by deleting the application instance.
 *
 * @return int Returns 0.
 */
int main()
{
    Moonstone::Core::Logger::Init();
    MS_INFO("application started successfully");

    auto app = Moonstone::Core::CreateApplicationInstance();
    app->Run();

    delete app;
}
