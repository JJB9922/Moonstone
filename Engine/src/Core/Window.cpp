#include "Include/Window.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

std::shared_ptr<spdlog::logger> logger = Logger::GetLoggerInstance();
EventDispatcher                 dispatcher;
EventQueue                      eventQueue(dispatcher);

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

    glfwSetWindowUserPointer(m_Window, &eventQueue);
    glfwMakeContextCurrent(m_Window);

    SetupWindowCallbacks(m_Window);
    SetupInitEvents();

    // TODO - Abstract for OpenGL and GLAD: gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(m_Window))

    {
        eventQueue.process();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    TerminateWindow();
}

void Window::SetupWindowCallbacks(GLFWwindow *window)
{
    glfwSetKeyCallback(m_Window,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
                           auto  event      = std::make_shared<KeyPressEvent>(key, action);
                           auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                           eventQueue->enqueue(event);
                       });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {});

    glfwSetWindowSizeCallback(m_Window,
                              [](GLFWwindow *window, int width, int height) {

                              });
}

void Window::SetupInitEvents()
{
    dispatcher.Subscribe(typeid(KeyPressEvent),
                         [](std::shared_ptr<Event> event)
                         {
                             auto keyEvent = std::static_pointer_cast<KeyPressEvent>(event);
                             int  key      = keyEvent->GetKeyCode();
                             int  action   = keyEvent->getAction();

                             switch (action)
                             {
                                 case GLFW_PRESS:
                                     MS_DEBUG("key pressed");
                                     break;
                                 case GLFW_RELEASE:
                                     break;
                                 case GLFW_REPEAT:
                                     break;
                             }
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
