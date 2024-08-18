#include "Include/Window.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

std::shared_ptr<spdlog::logger> logger = Logger::GetLoggerInstance();
std::unique_ptr<EventManager>   eventMgr = std::make_unique<EventManager>();

WindowProperties::WindowProperties(const std::string Title, unsigned Width, unsigned Height)
    : Title(Title)
    , Width(Width)
    , Height(Height)
{
}

Window *Window::CreateWindow(const WindowProperties &windowProperties)
{
    return new Window(windowProperties);
}

Window::Window(const WindowProperties &windowProperties) { InitializeWindow(windowProperties); }

Window::~Window() { TerminateWindow(); }

void Window::InitializeWindow(const WindowProperties &windowProperties)
{
    m_WindowData.windowProperties.Title  = windowProperties.Title;
    m_WindowData.windowProperties.Width  = windowProperties.Width;
    m_WindowData.windowProperties.Height = windowProperties.Height;

    glfwSetErrorCallback(ReportGLFWError);

    if (!glfwInit())
    {
        MS_ERROR("glfw initialization failed");
    }

    {
        // Logging window details
        std::stringstream ss;
        ss << "creating window: " << m_WindowData.windowProperties.Title << " - "
           << m_WindowData.windowProperties.Width << " x " << m_WindowData.windowProperties.Height
           << '\n';

        MS_INFO(ss.str());
    }

    m_Window = glfwCreateWindow((int) m_WindowData.windowProperties.Width,
                                (int) m_WindowData.windowProperties.Height,
                                m_WindowData.windowProperties.Title.c_str(),
                                nullptr,
                                nullptr);
    if (!m_Window)
    {
        MS_ERROR("window initialization failed");
    }

    glfwSetWindowUserPointer(m_Window, &m_WindowData);

    SetupWindowCallbacks(m_Window);

    glfwMakeContextCurrent(m_Window);

    // TODO - Abstract for OpenGL and GLAD: gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(1);

    while (true) {}

    while (!glfwWindowShouldClose(m_Window))

    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    TerminateWindow();
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        MS_INFO("KEY PRESSED");
}

void Window::SetupWindowCallbacks(GLFWwindow *window)
{
    glfwSetWindowCloseCallback(m_Window,
                               [](GLFWwindow *window)
                               {
                                   std::shared_ptr<WindowCloseEvent> event
                                       = std::make_shared<WindowCloseEvent>("WindowCloseEvent");

                                   std::unique_ptr<EventListener<WindowCloseEvent>> eventListener
                                       = std::make_unique<EventListener<WindowCloseEvent>>(*eventMgr,
                                                                                           event);

                                   eventMgr->Dispatch();
                                   eventListener->DetachEvent();
                               });

    glfwSetWindowSizeCallback(
        m_Window,
        [](GLFWwindow *window, int width, int height)
        {
            WindowData &data = *(WindowData *) glfwGetWindowUserPointer(window);

            std::shared_ptr<WindowResizeEvent> event
                = std::make_shared<WindowResizeEvent>("WindowCloseEvent",
                                                      data.windowProperties.Width,
                                                      data.windowProperties.Height);

            std::unique_ptr<EventListener<WindowResizeEvent>> eventListener
                = std::make_unique<EventListener<WindowResizeEvent>>(*eventMgr, event);

            eventMgr->Dispatch();
            eventListener->DetachEvent();
        });
}

void Window::ReportGLFWError(int error, const char *description)
{
    std::stringstream ss;
    ss << "error: " << description << '\n';
    MS_ERROR(ss.str());
}

void Window::TerminateWindow()
{
    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
    }

    glfwTerminate();
}

} // namespace Core

} // namespace Moonstone
