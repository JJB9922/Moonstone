#include "Include/Window.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

Moonstone::Core::EventDispatcher &eventDispatcher = Moonstone::Core::EventDispatcher::GetInstance();
Moonstone::Core::EventQueue      &eventQueue      = Moonstone::Core::EventQueue::GetInstance();
std::shared_ptr<spdlog::logger>   logger          = Logger::GetLoggerInstance();

Window *Window::s_MainWindow = nullptr;

WindowProperties::WindowProperties(const std::string Title, unsigned Width, unsigned Height)
    : Title(Title)
    , Width(Width)
    , Height(Height)
{
}

Window *Window::CreateWindow(const WindowProperties &windowProperties) { return new Window(windowProperties); }

Window::Window(const WindowProperties &windowProperties)
{
    InitializeWindow(windowProperties);

    if (!s_MainWindow)
    {
        s_MainWindow = this;
    }

    StartWindow();
}

Window::~Window() { TerminateWindow(); }

void Window::StartWindow()
{
    while (!glfwWindowShouldClose(m_Window))

    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        for (Layer *layer : m_LayerStack)
        {
            layer->OnUpdate();
        }

        m_ImGuiLayer->Start();
        for (Layer *layer : m_LayerStack)
        {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        eventQueue.process();
    }
}

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

    InitializeImGui();

    glfwSetWindowUserPointer(m_Window, &eventQueue);
    glfwMakeContextCurrent(m_Window);

    SetupWindowCallbacks(m_Window);
    SetupInputCallbacks(m_Window);
    SetupInitEvents();

    // TODO - Abstract for OpenGL and GLAD: gladLoadGL(glfwGetProcAddress);

    glfwSwapInterval(1);
}

void Window::InitializeImGui()
{
    m_ImGuiLayer = new Tools::ImGuiLayer();
    PushOverlay(m_ImGuiLayer);
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
    for (auto &event : m_SubscribedWindowEvents)
    {
        eventDispatcher.Unsubscribe(event);
    }

    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
    }

    glfwTerminate();
}

void Window::SetupInitEvents()
{
    eventDispatcher.Subscribe(typeid(KeyPressEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto keyEvent = std::static_pointer_cast<KeyPressEvent>(event);
                                  int  key      = keyEvent->GetKeyCode();
                                  int  action   = keyEvent->GetAction();

                                  std::stringstream ss;
                                  ss << "key press event: " << action << " - " << key;

                                  switch (action)
                                  {
                                      case GLFW_PRESS:
                                          MS_DEBUG(ss.str());
                                          break;
                                      case GLFW_RELEASE:
                                          MS_DEBUG(ss.str());
                                          break;
                                      case GLFW_REPEAT:
                                          MS_DEBUG(ss.str());
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

                                  std::stringstream ss;
                                  ss << "mouse button press event: " << action << " - " << btn;

                                  switch (action)
                                  {
                                      case GLFW_PRESS:
                                          MS_DEBUG(ss.str());
                                          break;
                                      case GLFW_RELEASE:
                                          MS_DEBUG(ss.str());
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

                                  std::stringstream ss;
                                  ss << "mouse scroll event: " << "x " << xOffset << " / y " << yOffset;
                                  MS_DEBUG(ss.str());
                              });

    m_SubscribedWindowEvents.push_back(typeid(MouseScrollEvent));

    eventDispatcher.Subscribe(typeid(MouseMoveEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto   moveEvent = std::static_pointer_cast<MouseMoveEvent>(event);
                                  double xPosition = moveEvent->GetXPosition();
                                  double yPosition = moveEvent->GetYPosition();

                                  std::stringstream ss;
                                  ss << "mouse move event: " << "x " << xPosition << " / y " << yPosition;
                                  MS_LOUD_DEBUG(ss.str());
                              });

    m_SubscribedWindowEvents.push_back(typeid(MouseMoveEvent));

    eventDispatcher.Subscribe(typeid(WindowCloseEvent),
                              [](std::shared_ptr<Event> event) { MS_DEBUG("window close event"); });

    m_SubscribedWindowEvents.push_back(typeid(WindowCloseEvent));

    eventDispatcher.Subscribe(typeid(WindowResizeEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto resizeEvent = std::static_pointer_cast<WindowResizeEvent>(event);
                                  int  width       = resizeEvent->GetWidth();
                                  int  height      = resizeEvent->GetHeight();

                                  std::stringstream ss;
                                  ss << "window resize event: " << width << " x " << height;
                                  MS_DEBUG(ss.str());
                              });

    m_SubscribedWindowEvents.push_back(typeid(WindowResizeEvent));

    eventDispatcher.Subscribe(typeid(WindowMinimizeEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto minimizeEvent = std::static_pointer_cast<WindowMinimizeEvent>(event);
                                  int  minimized     = minimizeEvent->IsMinimized();

                                  std::stringstream ss;
                                  ss << "window minimize event: " << minimized;
                                  MS_DEBUG(ss.str());
                              });

    m_SubscribedWindowEvents.push_back(typeid(WindowMinimizeEvent));

    eventDispatcher.Subscribe(typeid(WindowFocusEvent),
                              [](std::shared_ptr<Event> event)
                              {
                                  auto focusEvent = std::static_pointer_cast<WindowFocusEvent>(event);
                                  int  focused    = focusEvent->IsFocused();

                                  std::stringstream ss;
                                  ss << "window focus event: " << focused;
                                  MS_DEBUG(ss.str());
                              });

    m_SubscribedWindowEvents.push_back(typeid(WindowFocusEvent));
}

void Window::PushLayer(Layer *layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Window::PopLayer(Layer *layer) { m_LayerStack.PopLayer(layer); }

void Window::PushOverlay(Layer *layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Window::PopOverlay(Layer *overlay) { m_LayerStack.PopOverlay(overlay); }

} // namespace Core

} // namespace Moonstone
