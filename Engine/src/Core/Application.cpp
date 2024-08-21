#include "Include/Application.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

// ToDo: do this properly

Application* Application::s_ApplicationInstance = nullptr;

Application::Application()
{
    if (s_ApplicationInstance)
    {
        MS_ASSERT(!s_ApplicationInstance, "application instance already exists");
    }

    s_ApplicationInstance = this;
}

void Application::Run()
{
    m_Running = true;
    m_Window  = std::unique_ptr<Window>(Window::CreateWindow());

    unsigned shaderProgram, VBO, VAO, EBO;
    InitializeTestRenderData(shaderProgram, VBO, VAO, EBO);
    InitializeImGui();

    while (m_Running)
    {
        Renderer::RendererCommand::ClearColor(m_Window->m_WindowColor);
        Renderer::RendererCommand::Clear();

        glPolygonMode(GL_FRONT_AND_BACK, m_Window->m_GLPolygonMode);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        RenderLayers();

        Window::UpdateWindow(m_Window->m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void Application::RenderLayers()
{
    for (Layer* layer : m_LayerStack)
    {
        layer->OnUpdate();
    }

    m_ImGuiLayer->Start();
    for (Layer* layer : m_LayerStack)
    {
        layer->OnImGuiRender();
    }
    m_ImGuiLayer->End();
}

void Application::InitializeImGui()
{
    m_ImGuiLayer = new Tools::ImGuiLayer();
    m_ImGuiLayer->SetWindow(m_Window->m_Window);
    PushOverlay(m_ImGuiLayer);

    auto exampleLayer = new ExampleLayer;
    exampleLayer->SetBtnCallback(ExampleLayer::ButtonID::Exit, [this]() { m_Window->TerminateWindow(); });
    exampleLayer->SetBtnCallback(ExampleLayer::ButtonID::ApplyBGColor,
                                 [this, exampleLayer]()
                                 {
                                     auto color                = exampleLayer->GetBGColor();
                                     m_Window->m_WindowColor.r = color.x;
                                     m_Window->m_WindowColor.g = color.y;
                                     m_Window->m_WindowColor.b = color.z;
                                     m_Window->m_WindowColor.a = color.w;
                                 });
    exampleLayer->SetBtnCallback(ExampleLayer::ButtonID::ToggleWireframe,
                                 [this]()
                                 {
                                     if (m_Window->m_GLPolygonMode == GL_LINE)
                                     {
                                         m_Window->m_GLPolygonMode = GL_FILL;
                                     }
                                     else
                                     {
                                         m_Window->m_GLPolygonMode = GL_LINE;
                                     }
                                 });

    PushLayer(exampleLayer);
}

Application* CreateApplicationInstance() { return new Application(); }

void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Application::PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }

void Application::PushOverlay(Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void Application::PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }

void Application::InitializeTestRenderData(unsigned& shaderProgram, unsigned& VBO, unsigned& VAO, unsigned& EBO)
{
    auto vertexShaderSrc   = Renderer::BasicVertexShader::GetBasicVertexShaderSrc();
    auto fragmentShaderSrc = Renderer::BasicFragmentShader::GetBasicFragmentShaderSrc();

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
        return;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        MS_ERROR("fragment shader failed to compile: {0}", infoLog);
        return;
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
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {-0.5f,
                        -0.5f,
                        0.0f,

                        -0.25f,
                        0.0f,
                        0.0f,

                        0.0f,
                        -0.5f,
                        0.0f,

                        0.5f,
                        -0.5f,
                        0.0f,

                        0.25f,
                        0.0f,
                        0.0f,

                        0.0f,
                        0.5f,
                        0.0f

    };

    unsigned indices[] = {0,
                          1,
                          2,

                          2,
                          3,
                          4,

                          1,
                          4,
                          5};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

} // namespace Core
} // namespace Moonstone
