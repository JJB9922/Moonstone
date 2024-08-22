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
#include "Renderer/Include/RendererCommand.h"
#include "Tools/ImGui/Include/ImGuiLayer.h"
#include "mspch.h"

namespace Moonstone
{

namespace Core
{

struct GLColor
{
        float r, g, b, a;
};

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
        void           TerminateWindow();
        static void    UpdateWindow(GLFWwindow *window);

    public:
        glm::vec4   m_WindowColor;
        Renderer::RendererAPI::PolygonDataType m_PolygonMode;
        GLFWwindow *m_Window;

    private:
        static void ReportGLFWError(int error, const char *description);

        bool InitializeWindow(const WindowProperties &windowProperties);

        void        SetupWindowCallbacks(GLFWwindow *window);
        void        SetupInputCallbacks(GLFWwindow *window);
        void        SetupInitEvents();
        void        SetVSync(bool vSyncEnabled);

    private:
        WindowData                   m_WindowData;
        std::vector<std::type_index> m_SubscribedWindowEvents;
        Renderer::GraphicsContext   *m_GraphicsContext;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOW_H
