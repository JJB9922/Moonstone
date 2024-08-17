#include "mspch.h"

#include "Include/Application.h"

int main()
{
    std::cout << "Entry Point Hit" << '\n';
    auto app = Moonstone::Core::CreateApplicationInstance();
    app->Run();
    delete app;
}
