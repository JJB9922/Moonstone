#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Include/Logger.h"
#include "Events/Include/EventQueue.h"
#include "Events/Include/InputEvents.h"
#include "Events/Include/WindowEvents.h"
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
        inline void TerminateWindow();
        static void ReportGLFWError(int error, const char *description);
        void        InitializeWindow(const WindowProperties &windowProperties);
        void        SetupWindowCallbacks(GLFWwindow *window);
        void        SetupInputCallbacks(GLFWwindow *window);
        void        SetupInitEvents();

    private:
        WindowData  m_WindowData;
        GLFWwindow                 *m_Window;
        std::vector<std::type_info> m_SubscribedWindowEvents;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOW_H
