#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Include/LayerStack.h"
#include "Core/Include/Logger.h"
#include "Core/Layers/Include/BaseLayers.h"
#include "Events/Include/EventQueue.h"
#include "Events/Include/InputEvents.h"
#include "Events/Include/WindowEvents.h"
#include "Renderer/Include/GraphicsContext.h"
#include "Renderer/Include/GraphicsContextRouter.h"
#include "Tools/ImGui/Include/ImGuiLayer.h"
#include "mspch.h"

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

    private:
        inline void TerminateWindow();
        static void ReportGLFWError(int error, const char *description);

        bool InitializeWindow(const WindowProperties &windowProperties);
        void InitializeImGui();

        void StartWindow();
        void RenderLayers();

        void        SetupWindowCallbacks(GLFWwindow *window);
        void        SetupInputCallbacks(GLFWwindow *window);
        void        SetupInitEvents();

    private:
        LayerStack                   m_LayerStack;
        Tools::ImGuiLayer           *m_ImGuiLayer;
        WindowData                   m_WindowData;
        GLFWwindow                  *m_Window;
        std::vector<std::type_index> m_SubscribedWindowEvents;
        Renderer::GraphicsContext   *m_GraphicsContext;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOW_H
