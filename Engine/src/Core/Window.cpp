#include "Include/Window.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

Moonstone::Core::EventDispatcher &eventDispatcher = Moonstone::Core::EventDispatcher::GetInstance();
Moonstone::Core::EventQueue      &eventQueue      = Moonstone::Core::EventQueue::GetInstance();

std::shared_ptr<spdlog::logger> logger = Logger::GetLoggerInstance();

WindowProperties::WindowProperties(const std::string Title, unsigned Width, unsigned Height)
    : Title(Title)
    , Width(Width)
    , Height(Height)
{
}

Window *Window::CreateWindow(const WindowProperties &windowProperties) { return new Window(windowProperties); }

Window::Window(const WindowProperties &windowProperties)
{
    bool success = InitializeWindow(windowProperties);
    if (!success)
    {
        MS_ERROR("could not initialize window");
        TerminateWindow();
        return;
    }

    m_WindowColor = {0.7f, 0.75f, 0.78f, 1.0f};
}

Window::~Window() { glfwTerminate(); }

void Window::UpdateWindow(GLFWwindow *window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    eventQueue.process();
}

bool Window::InitializeWindow(const WindowProperties &windowProperties)
{
    m_WindowData.windowProperties.Title  = windowProperties.Title;
    m_WindowData.windowProperties.Width  = windowProperties.Width;
    m_WindowData.windowProperties.Height = windowProperties.Height;

    glfwSetErrorCallback(ReportGLFWError);

    if (!glfwInit())
    {
        MS_ERROR("glfw initialization failed");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    {
        MS_INFO("creating window: {0} - {1}x{2}",
                m_WindowData.windowProperties.Title,
                m_WindowData.windowProperties.Width,
                m_WindowData.windowProperties.Height);
    }

    m_Window = glfwCreateWindow((int) m_WindowData.windowProperties.Width,
                                (int) m_WindowData.windowProperties.Height,
                                m_WindowData.windowProperties.Title.c_str(),
                                nullptr,
                                nullptr);
    if (!m_Window)
    {
        MS_ERROR("window initialization failed");
        return false;
    }

    glfwSetWindowUserPointer(m_Window, &eventQueue);

    SetupInitEvents();
    SetupWindowCallbacks(m_Window);
    SetupInputCallbacks(m_Window);

    m_GraphicsContext = Renderer::GraphicsContextRouter::GetContext(m_Window);

    if (!m_GraphicsContext)
    {
        MS_ERROR("graphics context could not be set");
        return false;
    }

    m_GraphicsContext->Init();
    SetVSync(true);

    return true;
}

void Window::SetVSync(bool vSyncEnabled)
{
    if (vSyncEnabled)
    {
        glfwSwapInterval(1);
    }
    else
    {
        glfwSwapInterval(0);
    }

    m_WindowData.VSync = vSyncEnabled;
}

void Window::SetupInputCallbacks(GLFWwindow *window)
{
    glfwSetKeyCallback(m_Window,
                       [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
                           auto  event      = std::make_shared<KeyPressEvent>(key, action);
                           auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                           eventQueue->enqueue(event);
                       });

    glfwSetMouseButtonCallback(m_Window,
                               [](GLFWwindow *window, int button, int action, int mods)
                               {
                                   auto  event      = std::make_shared<MouseButtonPressEvent>(button, action);
                                   auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                   eventQueue->enqueue(event);
                               });

    glfwSetScrollCallback(m_Window,
                          [](GLFWwindow *window, double xOffset, double yOffset)
                          {
                              auto  event      = std::make_shared<MouseScrollEvent>(xOffset, yOffset);
                              auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                              eventQueue->enqueue(event);
                          });

    glfwSetCursorPosCallback(m_Window,
                             [](GLFWwindow *window, double xPosition, double yPosition)
                             {
                                 auto  event      = std::make_shared<MouseMoveEvent>(xPosition, yPosition);
                                 auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                 eventQueue->enqueue(event);
                             });
}

void Window::SetupWindowCallbacks(GLFWwindow *window)
{
    glfwSetWindowCloseCallback(m_Window,
                               [](GLFWwindow *window)
                               {
                                   auto  event      = std::make_shared<WindowCloseEvent>();
                                   auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                   eventQueue->enqueue(event);
                               });

    glfwSetWindowSizeCallback(m_Window,
                              [](GLFWwindow *window, int width, int height)
                              {
                                  auto  event      = std::make_shared<WindowResizeEvent>(width, height);
                                  auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                  eventQueue->enqueue(event);
                              });

    glfwSetWindowIconifyCallback(m_Window,
                                 [](GLFWwindow *window, int minimized)
                                 {
                                     auto  event      = std::make_shared<WindowMinimizeEvent>(minimized);
                                     auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                     eventQueue->enqueue(event);
                                 });

    glfwSetWindowFocusCallback(m_Window,
                               [](GLFWwindow *window, int focused)
                               {
                                   auto  event      = std::make_shared<WindowFocusEvent>(focused);
                                   auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                   eventQueue->enqueue(event);
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
    glfwSetWindowShouldClose(m_Window, true);

    for (auto &event : m_SubscribedWindowEvents)
    {
        eventDispatcher.Unsubscribe(event);
    }
}

void Window::SetupInitEvents()
{
    eventDispatcher.Subscribe(typeid(KeyPressEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto keyEvent = std::static_pointer_cast<KeyPressEvent>(event);
                                  int  key      = keyEvent->GetKeyCode();
                                  int  action   = keyEvent->GetAction();

                                  switch (action)
                                  {
                                      case GLFW_PRESS:
                                          MS_LOUD_DEBUG("key press event: {0} - {1}", action, key);
                                          break;
                                      case GLFW_RELEASE:
                                          MS_LOUD_DEBUG("key release event: {0} - {1}", action, key);
                                          break;
                                      case GLFW_REPEAT:
                                          MS_LOUD_DEBUG("key repeat event: {0} - {1}", action, key);
                                          break;
                                  }
                              });

    m_SubscribedWindowEvents.push_back(typeid(KeyPressEvent));

    eventDispatcher.Subscribe(typeid(MouseButtonPressEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto btnEvent = std::static_pointer_cast<MouseButtonPressEvent>(event);
                                  int  btn      = btnEvent->GetButton();
                                  int  action   = btnEvent->GetAction();

                                  switch (action)
                                  {
                                      case GLFW_PRESS:
                                          MS_LOUD_DEBUG("mouse button press event: {0} - {1}", action, btn);
                                          break;
                                      case GLFW_RELEASE:
                                          MS_LOUD_DEBUG("mouse button release event: {0} - {1}", action, btn);
                                          break;
                                  }
                              });

    m_SubscribedWindowEvents.push_back(typeid(MouseButtonPressEvent));

    eventDispatcher.Subscribe(typeid(MouseScrollEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto scrollEvent = std::static_pointer_cast<MouseScrollEvent>(event);
                                  int  xOffset     = scrollEvent->GetXOffset();
                                  int  yOffset     = scrollEvent->GetYOffset();

                                  MS_DEBUG("mouse scroll event: x{0}, y{1}", xOffset, yOffset);
                              });

    m_SubscribedWindowEvents.push_back(typeid(MouseScrollEvent));

    eventDispatcher.Subscribe(typeid(MouseMoveEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto   moveEvent = std::static_pointer_cast<MouseMoveEvent>(event);
                                  double xPosition = moveEvent->GetXPosition();
                                  double yPosition = moveEvent->GetYPosition();

                                  MS_LOUD_DEBUG("mouse move event: x{0}, y{1}", xPosition, yPosition);
                              });

    m_SubscribedWindowEvents.push_back(typeid(MouseMoveEvent));

    eventDispatcher.Subscribe(typeid(WindowCloseEvent),
                              [this](std::shared_ptr<Event> event)
                              {
                                  MS_DEBUG("window close event");
                                  TerminateWindow();
                              });

    m_SubscribedWindowEvents.push_back(typeid(WindowCloseEvent));

    eventDispatcher.Subscribe(typeid(WindowResizeEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto resizeEvent = std::static_pointer_cast<WindowResizeEvent>(event);
                                  int  width       = resizeEvent->GetWidth();
                                  int  height      = resizeEvent->GetHeight();

                                  // ToDo : Don't forget this abstraction
                                  glViewport(0, 0, width, height);
                                  MS_DEBUG("window resize event: {0}x{1}", width, height);
                              });

    m_SubscribedWindowEvents.push_back(typeid(WindowResizeEvent));

    eventDispatcher.Subscribe(typeid(WindowMinimizeEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto minimizeEvent = std::static_pointer_cast<WindowMinimizeEvent>(event);
                                  int  minimized     = minimizeEvent->IsMinimized();

                                  MS_DEBUG("window minimize event: {0}", minimized);
                              });

    m_SubscribedWindowEvents.push_back(typeid(WindowMinimizeEvent));

    eventDispatcher.Subscribe(typeid(WindowFocusEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto focusEvent = std::static_pointer_cast<WindowFocusEvent>(event);
                                  int  focused    = focusEvent->IsFocused();

                                  MS_LOUD_DEBUG("window focus event: {0}", focused);
                              });

    m_SubscribedWindowEvents.push_back(typeid(WindowFocusEvent));
}

} // namespace Core

} // namespace Moonstone
