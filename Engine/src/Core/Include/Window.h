#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Include/LayerStack.h"
#include "Core/Include/Logger.h"
#include "Events/Include/EventQueue.h"
#include "Events/Include/InputEvents.h"
#include "Events/Include/WindowEvents.h"
#include "Tools/ImGui/Include/ImGuiLayer.h"
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

        void PushLayer(Layer *layer);
        void PopLayer(Layer *layer);
        void PushOverlay(Layer *overlay);
        void PopOverlay(Layer *overlay);

        static GLFWwindow *GetMainWindow()
        {
            if (s_MainWindow)
            {
                return s_MainWindow->m_Window;
            }
            return nullptr;
        }

    private:
        inline void TerminateWindow();
        static void ReportGLFWError(int error, const char *description);

        void InitializeWindow(const WindowProperties &windowProperties);
        void InitializeImGui();

        void StartWindow();

        void        SetupWindowCallbacks(GLFWwindow *window);
        void        SetupInputCallbacks(GLFWwindow *window);
        void        SetupInitEvents();

    private:
        LayerStack                   m_LayerStack;
        Tools::ImGuiLayer           *m_ImGuiLayer;
        WindowData                   m_WindowData;
        GLFWwindow                  *m_Window;
        static Window               *s_MainWindow;
        std::vector<std::type_index> m_SubscribedWindowEvents;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOW_H
