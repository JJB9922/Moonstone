#include "Include/Application.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{
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

        Renderer::RendererCommand::SubmitDrawCommands(shaderProgram, VAO);

        RenderLayers();

        Window::UpdateWindow(m_Window->m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    Renderer::RendererCommand::Cleanup(VAO, VBO, shaderProgram);
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
                                     if (m_Window->m_PolygonMode == Renderer::RendererAPI::DataType::PolygonLine)
                                     {
                                         Renderer::RendererCommand::SetPolygonMode(
                                             Renderer::RendererAPI::DataType::PolygonFill);

                                         m_Window->m_PolygonMode = Renderer::RendererAPI::DataType::PolygonFill;
                                     }
                                     else
                                     {
                                         Renderer::RendererCommand::SetPolygonMode(
                                             Renderer::RendererAPI::DataType::PolygonLine);

                                         m_Window->m_PolygonMode = Renderer::RendererAPI::DataType::PolygonLine;
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

    unsigned vertexShader, fragmentShader;
    Renderer::RendererCommand::InitVertexShader(vertexShader, vertexShaderSrc);
    Renderer::RendererCommand::InitFragmentShader(fragmentShader, fragmentShaderSrc);
    Renderer::RendererCommand::InitShaderProgram(shaderProgram, vertexShader, fragmentShader);

    Renderer::RendererCommand::InitVertexArray(VAO);
    Renderer::RendererCommand::InitVertexBuffer(VBO, vertices, sizeof(vertices));
    Renderer::RendererCommand::InitElementBuffer(EBO, indices, sizeof(indices));
    Renderer::RendererCommand::InitVertexAttributes();
}

} // namespace Core
} // namespace Moonstone
