#ifndef WINDOW_H
#define WINDOW_H

#include "Rendering/Include/CameraController.h"
#include "Tools/ImGui/Include/ImGuiLayer.h"

#include "Core/Layers/Include/BaseLayers.h"

#include "Core/Include/LayerStack.h"
#include "Core/Include/Logger.h"
#include "Events/Include/EventQueue.h"
#include "Events/Include/InputEvents.h"
#include "Events/Include/WindowEvents.h"
#include "Rendering/Include/Camera.h"
#include "Rendering/Include/GraphicsContext.h"
#include "Rendering/Include/GraphicsContextRouter.h"
#include "Rendering/Include/RenderingCommand.h"
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
    unsigned Width, Height;

    WindowProperties(const std::string Title = "Moonstone", unsigned Width = 1280, unsigned Height = 720);
};

struct WindowData
{
    WindowProperties windowProperties;
    bool VSync;
};

class Window
{
  public:
    Window(const WindowProperties &windowProperties);
    virtual ~Window();

    static std::unique_ptr<Window> CreateWindow(const WindowProperties &windowProperties = WindowProperties());
    void TerminateWindow();
    static void UpdateWindow(std::shared_ptr<Window> window);

    inline void SetCamera(std::shared_ptr<Rendering::CameraController> camera)
    {
        m_CameraController = camera;
    }

    inline std::shared_ptr<Rendering::CameraController> GetCamera()
    {
        return m_CameraController;
    }

    inline void SetCameraSens(float sens)
    {
        m_CamSensitivity = sens;
    }

    inline static int GetWidth()
    {
        return WindowProperties().Width;
    }
    inline static int GetHeight()
    {
        return WindowProperties().Height;
    }

  public:
    glm::vec4 m_WindowColor;
    Rendering::RenderingAPI::PolygonDataType m_PolygonMode;
    GLFWwindow *m_Window;

  private:
    static void ReportGLFWError(int error, const char *description);

    bool InitializeWindow(const WindowProperties &windowProperties);

    void SetupWindowCallbacks(GLFWwindow *window);
    void SetupInputCallbacks(GLFWwindow *window);
    void SetupInitEvents();
    void SetVSync(bool vSyncEnabled);

  private:
    WindowData m_WindowData;
    std::vector<std::type_index> m_SubscribedWindowEvents;
    std::unique_ptr<Rendering::GraphicsContext> m_GraphicsContext;
    std::shared_ptr<Rendering::CameraController> m_CameraController = nullptr;
    float m_LastX = m_WindowData.windowProperties.Width;
    float m_LastY = m_WindowData.windowProperties.Height;
    bool m_FirstMouse = true;
    float m_CamSensitivity = 0.2f;

    std::shared_ptr<EventDispatcher> m_EventDispatcher;
    std::shared_ptr<EventQueue> m_EventQueue;
    std::shared_ptr<spdlog::logger> m_Logger;
};

} // namespace Core

} // namespace Moonstone

#endif // WINDOW_H
