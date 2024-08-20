#include "Include/Window.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

Moonstone::Core::EventDispatcher &eventDispatcher = Moonstone::Core::EventDispatcher::GetInstance();
Moonstone::Core::EventQueue      &eventQueue      = Moonstone::Core::EventQueue::GetInstance();
std::shared_ptr<spdlog::logger>   logger          = Logger::GetLoggerInstance();

// ToDo: do this properly
unsigned int shaderProgram, VBO, VAO;

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

    InitializeTestRenderData();
    StartWindow();
}

Window::~Window() { glfwTerminate(); }

void Window::InitializeTestRenderData()
{
    auto vertexShaderSrc = R"(
                          #version 330 core
                          layout (location = 0) in vec3 aPos;
                          
                          void main()
                          {
                              gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                          }
                          
                          )";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        MS_ERROR("vertex shader failed to compile: {0}", infoLog);
    }

    auto fragmentShaderSrc = R"(

        #version 330 core
        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(0.83f, 0.52f, 0.95f, 1.0f);
        }
                          
                          )";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        MS_ERROR("fragment shader failed to compile: {0}", infoLog);
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        MS_ERROR("shader program failed to link: {0}", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Window::StartWindow()
{
    while (!glfwWindowShouldClose(m_Window))

    {
        Renderer::RendererCommand::ClearColor(m_WindowColor);
        Renderer::RendererCommand::Clear();

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        RenderLayers();
        glfwSwapBuffers(m_Window);
        glfwPollEvents();

        eventQueue.process();
    }
}

void Window::RenderLayers()
{
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

    InitializeImGui();

    glfwSwapInterval(1);

    return true;
}

void Window::InitializeImGui()
{
    m_ImGuiLayer = new Tools::ImGuiLayer();
    m_ImGuiLayer->SetWindow(m_Window);
    PushOverlay(m_ImGuiLayer);

    auto exampleLayer = new ExampleLayer;
    exampleLayer->SetBtnCallback(ExampleLayer::ButtonID::Exit, [this]() { TerminateWindow(); });
    exampleLayer->SetBtnCallback(ExampleLayer::ButtonID::ApplyBGColor,
                                 [this, exampleLayer]()
                                 {
                                     auto color      = exampleLayer->GetBGColor();
                                     m_WindowColor.r = color.x;
                                     m_WindowColor.g = color.y;
                                     m_WindowColor.b = color.z;
                                     m_WindowColor.a = color.w;
                                 });

    PushLayer(exampleLayer);
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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
    }

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
