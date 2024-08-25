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

Window *Window::CreateWindow(const WindowProperties &windowProperties) { return new Window(windowProperties); }

Window::Window(const WindowProperties &windowProperties)
{
    m_EventDispatcher = EventDispatcher::GetEventDispatcherInstance();
    m_EventQueue      = EventQueue::GetEventQueueInstance();
    m_Logger          = Logger::GetLoggerInstance();

    bool success = InitializeWindow(windowProperties);
    if (!success)
    {
        MS_ERROR("could not initialize window");
        TerminateWindow();
        return;
    }

    m_WindowColor = {0.15f, 0.15f, 0.15f, 1.0f};
}

Window::~Window() { glfwTerminate(); }

void Window::UpdateWindow(std::shared_ptr<Window> window)
{
    glfwSwapBuffers(window->m_Window);
    glfwPollEvents();
    window->m_EventQueue->Process();
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

    glfwSetWindowUserPointer(m_Window, m_EventQueue.get());

    SetupInitEvents();
    SetupWindowCallbacks(m_Window);
    SetupInputCallbacks(m_Window);

    m_GraphicsContext = Renderer::GraphicsContextRouter::GetContext(m_Window);

    if (!m_GraphicsContext)
    {
        MS_ERROR("graphics context could not be set");
        return false;
    }

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
                           auto  event      = std::make_shared<KeyPressEvent>(key, action, mods);
                           auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                           eventQueue->Enqueue(event);
                       });

    glfwSetMouseButtonCallback(m_Window,
                               [](GLFWwindow *window, int button, int action, int mods)
                               {
                                   auto  event      = std::make_shared<MouseButtonPressEvent>(button, action, mods);
                                   auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                   eventQueue->Enqueue(event);
                               });

    glfwSetScrollCallback(m_Window,
                          [](GLFWwindow *window, double xOffset, double yOffset)
                          {
                              auto  event      = std::make_shared<MouseScrollEvent>(xOffset, yOffset);
                              auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                              eventQueue->Enqueue(event);
                          });

    glfwSetCursorPosCallback(m_Window,
                             [](GLFWwindow *window, double xPosition, double yPosition)
                             {
                                 auto  event      = std::make_shared<MouseMoveEvent>(xPosition, yPosition);
                                 auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                 eventQueue->Enqueue(event);
                             });
}

void Window::SetupWindowCallbacks(GLFWwindow *window)
{
    glfwSetWindowCloseCallback(m_Window,
                               [](GLFWwindow *window)
                               {
                                   auto  event      = std::make_shared<WindowCloseEvent>();
                                   auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                   eventQueue->Enqueue(event);
                               });

    glfwSetWindowSizeCallback(m_Window,
                              [](GLFWwindow *window, int width, int height)
                              {
                                  auto  event      = std::make_shared<WindowResizeEvent>(width, height);
                                  auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                  eventQueue->Enqueue(event);
                              });

    glfwSetWindowIconifyCallback(m_Window,
                                 [](GLFWwindow *window, int minimized)
                                 {
                                     auto  event      = std::make_shared<WindowMinimizeEvent>(minimized);
                                     auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                     eventQueue->Enqueue(event);
                                 });

    glfwSetWindowFocusCallback(m_Window,
                               [](GLFWwindow *window, int focused)
                               {
                                   auto  event      = std::make_shared<WindowFocusEvent>(focused);
                                   auto *eventQueue = static_cast<EventQueue *>(glfwGetWindowUserPointer(window));
                                   eventQueue->Enqueue(event);
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
        m_EventDispatcher->Unsubscribe(event);
    }
}

void Window::SetupInitEvents()
{
    m_EventDispatcher->Subscribe(typeid(KeyPressEvent),
                                 [this](std::shared_ptr<Event> event)
                                 {
                                     auto keyEvent = std::static_pointer_cast<KeyPressEvent>(event);
                                     int  key      = keyEvent->GetKeyCode();
                                     int  action   = keyEvent->GetAction();
                                     int  mods     = keyEvent->GetMods();

                                     switch (action)
                                     {
                                         case GLFW_PRESS:
                                             MS_LOUD_DEBUG("key press event: {0} - {1}", action, key);

                                             if (m_CameraController && m_CameraController->GetConnected())
                                             {
                                                 if (key == GLFW_KEY_W)
                                                 {
                                                     m_CameraController->OnMoveForward();
                                                 }
                                                 else if (key == GLFW_KEY_S)
                                                 {
                                                     m_CameraController->OnMoveBackward();
                                                 }
                                                 else if (key == GLFW_KEY_A)
                                                 {
                                                     m_CameraController->OnMoveLeft();
                                                 }
                                                 else if (key == GLFW_KEY_D)
                                                 {
                                                     m_CameraController->OnMoveRight();
                                                 }
                                             }

                                             if (key == GLFW_KEY_ESCAPE && m_CameraController->GetConnected())
                                             {
                                                 glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                                                 m_CameraController->SetConnected(false);
                                                 m_FirstMouse = true;
                                             }
                                             else if (key == GLFW_KEY_ESCAPE && !m_CameraController->GetConnected())
                                             {
                                                 glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                                                 m_CameraController->SetConnected(true);
                                             }

                                             break;
                                         case GLFW_RELEASE:
                                             MS_LOUD_DEBUG("key release event: {0} - {1}", action, key);
                                             break;
                                         case GLFW_REPEAT:
                                             MS_LOUD_DEBUG("key repeat event: {0} - {1}", action, key);

                                             if (m_CameraController && m_CameraController->GetConnected())
                                             {
                                                 if (key == GLFW_KEY_W)
                                                 {
                                                     m_CameraController->OnMoveForward();
                                                 }
                                                 else if (key == GLFW_KEY_S)
                                                 {
                                                     m_CameraController->OnMoveBackward();
                                                 }
                                                 else if (key == GLFW_KEY_A)
                                                 {
                                                     m_CameraController->OnMoveLeft();
                                                 }
                                                 else if (key == GLFW_KEY_D)
                                                 {
                                                     m_CameraController->OnMoveRight();
                                                 }
                                             }

                                             break;
                                     }
                                 });

    m_SubscribedWindowEvents.push_back(typeid(KeyPressEvent));

    m_EventDispatcher->Subscribe(typeid(MouseButtonPressEvent),
                                 [this](std::shared_ptr<Event> event)
                                 {
                                     auto btnEvent = std::static_pointer_cast<MouseButtonPressEvent>(event);
                                     int  btn      = btnEvent->GetButton();
                                     int  action   = btnEvent->GetAction();

                                     switch (action)
                                     {
                                         case GLFW_PRESS:
                                             MS_DEBUG("mouse button press event: {0} - {1}", action, btn);

                                             if (m_CameraController && m_CameraController->GetConnected())
                                             {
                                                 if (btn == GLFW_MOUSE_BUTTON_MIDDLE)
                                                 {
                                                     m_CameraController->SetFov(65.0f);
                                                 }
                                             }

                                             break;
                                         case GLFW_RELEASE:
                                             MS_LOUD_DEBUG("mouse button release event: {0} - {1}", action, btn);
                                             break;
                                     }
                                 });

    m_SubscribedWindowEvents.push_back(typeid(MouseButtonPressEvent));

    m_EventDispatcher->Subscribe(typeid(MouseScrollEvent),
                                 [this](std::shared_ptr<Event> event)
                                 {
                                     auto scrollEvent = std::static_pointer_cast<MouseScrollEvent>(event);
                                     int  xOffset     = scrollEvent->GetXOffset();
                                     int  yOffset     = scrollEvent->GetYOffset();

                                     if (m_CameraController && m_CameraController->GetConnected())
                                     {
                                         m_CameraController->SetFov(m_CameraController->GetFov() - (float) yOffset);

                                         if (m_CameraController->GetFov() < 1.0f)
                                             m_CameraController->SetFov(1.0f);
                                         if (m_CameraController->GetFov() > 120.0f)
                                             m_CameraController->SetFov(120.0f);
                                     }

                                     MS_DEBUG("mouse scroll event: x{0}, y{1}", xOffset, yOffset);
                                 });

    m_SubscribedWindowEvents.push_back(typeid(MouseScrollEvent));

    m_EventDispatcher->Subscribe(typeid(MouseMoveEvent),
                                 [this](std::shared_ptr<Event> event)
                                 {
                                     auto   moveEvent = std::static_pointer_cast<MouseMoveEvent>(event);
                                     double xPosition = moveEvent->GetXPosition();
                                     double yPosition = moveEvent->GetYPosition();

                                     if (m_CameraController && m_CameraController->GetConnected())
                                     {
                                         if (m_FirstMouse)
                                         {
                                             m_LastX      = xPosition;
                                             m_LastY      = yPosition;
                                             m_FirstMouse = false;
                                         }

                                         float xoffset = xPosition - m_LastX;
                                         float yoffset = m_LastY - yPosition;
                                         m_LastX       = xPosition;
                                         m_LastY       = yPosition;

                                         xoffset *= m_CamSensitivity;
                                         yoffset *= m_CamSensitivity;

                                         m_CameraController->SetPitch(m_CameraController->GetPitch() + yoffset);
                                         m_CameraController->SetYaw(m_CameraController->GetYaw() + xoffset);

                                         if (m_CameraController->GetPitch() > 89.0f)
                                             m_CameraController->SetPitch(89.0f);
                                         if (m_CameraController->GetPitch() < -89.0f)
                                             m_CameraController->SetPitch(-89.0f);

                                         glm::vec3 direction;
                                         direction.x = cos(glm::radians(m_CameraController->GetYaw()))
                                                       * cos(glm::radians(m_CameraController->GetPitch()));
                                         direction.y = sin(glm::radians(m_CameraController->GetPitch()));
                                         direction.z = sin(glm::radians(m_CameraController->GetYaw()))
                                                       * cos(glm::radians(m_CameraController->GetPitch()));

                                         // Normalize the direction vector
                                         m_CameraController->SetDirection(glm::normalize(direction));
                                         m_CameraController->SetFront(m_CameraController->GetDirection());
                                     }

                                     MS_LOUD_DEBUG("mouse move event: x{0}, y{1}", xPosition, yPosition);
                                 });

    m_SubscribedWindowEvents.push_back(typeid(MouseMoveEvent));

    m_EventDispatcher->Subscribe(typeid(WindowCloseEvent),
                                 [this](std::shared_ptr<Event> event)
                                 {
                                     MS_DEBUG("window close event");
                                     TerminateWindow();
                                 });

    m_SubscribedWindowEvents.push_back(typeid(WindowCloseEvent));

    m_EventDispatcher->Subscribe(typeid(WindowResizeEvent),
                                 [](std::shared_ptr<Event> event)
                                 {
                                     auto resizeEvent = std::static_pointer_cast<WindowResizeEvent>(event);
                                     int  width       = resizeEvent->GetWidth();
                                     int  height      = resizeEvent->GetHeight();

                                     Renderer::RendererCommand::SetViewport(width, height);
                                     MS_DEBUG("window resize event: {0}x{1}", width, height);
                                 });

    m_SubscribedWindowEvents.push_back(typeid(WindowResizeEvent));

    m_EventDispatcher->Subscribe(typeid(WindowMinimizeEvent),
                                 [](std::shared_ptr<Event> event)
                                 {
                                     auto minimizeEvent = std::static_pointer_cast<WindowMinimizeEvent>(event);
                                     int  minimized     = minimizeEvent->IsMinimized();

                                     MS_DEBUG("window minimize event: {0}", minimized);
                                 });

    m_SubscribedWindowEvents.push_back(typeid(WindowMinimizeEvent));

    m_EventDispatcher->Subscribe(typeid(WindowFocusEvent),
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
