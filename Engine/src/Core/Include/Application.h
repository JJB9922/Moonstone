#ifndef APPLICATION_H
#define APPLICATION_H

namespace Moonstone
{

namespace Core
{

class Application
{
public:
    Application();
    virtual ~Application() = default;

    void Run();

    inline static Application& GetApplicationInstance() { return *s_ApplicationInstance; }

private:
    static Application* s_ApplicationInstance;
};

Application* CreateApplicationInstance();

} // namespace Core

} // namespace Moonstone

#endif // APPLICATION_H
