#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Events/Include/EventListener.h"
#include "Core/Events/Include/WindowEvent.h"
#include "Core/Include/Logger.h"
#include "mspch.h"
#include <GLFW/glfw3.h>

namespace Moonstone
{

namespace Core
{

struct WindowProperties
{
        std::string Title;
        unsigned    Width, Height;

        WindowProperties(const std::string Title  = "Moonstone",
                         unsigned          Width  = 1280,
                         unsigned          Height = 720);
};

struct WindowData
{
        WindowProperties windowProperties;
        bool             VSync;
};

class Window
{
    public:
        Window(const WindowProperties &windowProperties);
        virtual ~Window();

        static Window *CreateWindow(const WindowProperties &windowProperties = WindowProperties());

    private:
        inline void InitializeWindow(const WindowProperties &windowProperties);
        inline void SetupWindowCallbacks(GLFWwindow *window);
        inline void TerminateWindow();
        static void ReportGLFWError(int error, const char *description);

    private:
        WindowData  m_WindowData;
        GLFWwindow *m_Window;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOW_H
