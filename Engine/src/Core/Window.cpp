#include "Include/Window.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

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

    std::cout << "creating window" << '\n';
    glfwSetErrorCallback(ReportGLFWError);

    if (!glfwInit())
    {
        std::cerr << "glfw initialisation failed" << '\n';
    }

    m_Window = glfwCreateWindow((int) m_WindowData.windowProperties.Width,
                                (int) m_WindowData.windowProperties.Height,
                                m_WindowData.windowProperties.Title.c_str(),
                                nullptr,
                                nullptr);
    if (!m_Window)
    {
        std::cerr << "window initialisation failed" << '\n';
    }

    glfwMakeContextCurrent(m_Window);

    // TODO - Abstract for OpenGL and GLAD: gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(1);

    while (true) {}

    while (!glfwWindowShouldClose(m_Window))
    {
        UpdateWindow(m_Window);
    }

    TerminateWindow();
}

void Window::UpdateWindow(GLFWwindow *window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::ReportGLFWError(int error, const char *description)
{
    std::cerr << "Error: " << description << '\n';
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
