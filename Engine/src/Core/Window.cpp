#include "Include/Window.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

Moonstone::Core::EventDispatcher &eventDispatcher = Moonstone::Core::EventDispatcher::GetInstance();
Moonstone::Core::EventQueue      &eventQueue      = Moonstone::Core::EventQueue::GetInstance();

std::shared_ptr<spdlog::logger> logger = Logger::GetLoggerInstance();

/**
 * @brief Constructs a WindowProperties instance.
 *
 * Initializes the window properties with the specified title, width, and height.
 *
 * @param Title The title of the window.
 * @param Width The width of the window.
 * @param Height The height of the window.
 */
WindowProperties::WindowProperties(const std::string Title, unsigned Width, unsigned Height)
    : Title(Title)
    , Width(Width)
    , Height(Height)
{
}

/**
 * @brief Creates a new Window instance.
 *
 * Instantiates a new Window object with the given properties.
 *
 * @param windowProperties The properties to use for the new window.
 * @return Window* A pointer to the newly created Window instance.
 */
Window *Window::CreateWindow(const WindowProperties &windowProperties) { return new Window(windowProperties); }

/**
 * @brief Constructs a Window instance.
 *
 * Initializes the window using the provided properties and sets up the window
 * color. Logs an error and terminates initialization if the window fails to
 * initialize.
 *
 * @param windowProperties The properties to use for initializing the window.
 */
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

/**
 * @brief Destructs a Window instance.
 *
 * Terminates GLFW and cleans up resources associated with the window.
 */
Window::~Window() { glfwTerminate(); }

/**
 * @brief Updates the window.
 *
 * Swaps the front and back buffers and processes events for the window.
 *
 * @param window The GLFW window to update.
 */
void Window::UpdateWindow(GLFWwindow *window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    eventQueue.process();
}

/**
 * @brief Initializes the window.
 *
 * Sets up GLFW, creates the window, and initializes the graphics context.
 * Logs errors and performs clean-up if any step fails.
 *
 * @param windowProperties The properties to use for initializing the window.
 * @return bool True if the window was initialized successfully; false otherwise.
 */
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

/**
 * @brief Sets the vertical synchronization (V-Sync) mode.
 *
 * Enables or disables V-Sync based on the provided flag.
 *
 * @param vSyncEnabled True to enable V-Sync; false to disable it.
 */
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

/**
 * @brief Sets up input callbacks for the window.
 *
 * Configures GLFW callbacks for keyboard, mouse button, scroll, and cursor position events.
 *
 * @param window The GLFW window to configure callbacks for.
 */
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

/**
 * @brief Sets up window-related callbacks.
 *
 * Configures GLFW callbacks for window close, resize, minimize, and focus events.
 *
 * @param window The GLFW window to configure callbacks for.
 */
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

/**
 * @brief Reports GLFW errors.
 *
 * Logs GLFW error messages using the provided error code and description.
 *
 * @param error The error code.
 * @param description The error description.
 */
void Window::ReportGLFWError(int error, const char *description)
{
    std::stringstream ss;
    ss << "error: " << description << '\n';
    MS_ERROR(ss.str());
}

/**
 * @brief Terminates the window.
 *
 * Sets the window to close and unsubscribes all events from the event dispatcher.
 */
void Window::TerminateWindow()
{
    glfwSetWindowShouldClose(m_Window, true);

    for (auto &event : m_SubscribedWindowEvents)
    {
        eventDispatcher.Unsubscribe(event);
    }
}

/**
 * @brief Sets up initial events for the window.
 *
 * Subscribes to various window events (e.g., key presses, mouse clicks) and
 * configures their corresponding event handlers.
 */
void Window::SetupInitEvents()
{
    eventDispatcher.Subscribe(typeid(KeyPressEvent),
                              [this](std::shared_ptr<Event> event)
                              {
                                  auto keyEvent = std::static_pointer_cast<KeyPressEvent>(event);
                                  int  key      = keyEvent->GetKeyCode();
                                  int  action   = keyEvent->GetAction();

                                  switch (action)
                                  {
                                      case GLFW_PRESS:
                                          MS_LOUD_DEBUG("key press event: {0} - {1}", action, key);

                                          if (m_CameraController)
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
