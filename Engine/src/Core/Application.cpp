#include "Include/Application.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

float gridVertices[] = {
    -50.0f, 0.0f, -50.0f, 50.0f,  0.0f, -50.0f,

    -50.0f, 0.0f, 50.0f,  50.0f,  0.0f, 50.0f,

    -50.0f, 0.0f, -50.0f, -50.0f, 0.0f, 50.0f,

    50.0f,  0.0f, -50.0f, 50.0f,  0.0f, 50.0f,
};

float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

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
    m_Window  = std::shared_ptr<Window>(Window::CreateWindow());

    glm::vec3 cameraPos   = glm::vec3(0.0f, 4.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    float nearClip = 0.1f;
    float farClip  = 100.0f;

    auto pCamera = std::make_shared<Renderer::Camera>(cameraPos, cameraFront, cameraUp);

    m_Window->SetCamera(pCamera);

    unsigned shaderProgram[16], VBO[16], VAO[16], EBO[16], texture[16];

    std::string      gridVert = std::string(RESOURCE_DIR) + "/Shaders/defaultgrid.vert";
    std::string      gridFrag = std::string(RESOURCE_DIR) + "/Shaders/defaultgrid.frag";
    Renderer::Shader gridShader(gridVert.c_str(), gridFrag.c_str());

    std::string      cubeVert = std::string(RESOURCE_DIR) + "/Shaders/lightvshader.vs";
    std::string      cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/lightfshader.fs";
    Renderer::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    InitializeDefaultGrid(shaderProgram, VBO, VAO, EBO, texture);
    InitializeImGui();

    Time& time = Time::GetInstance();

    while (m_Running)
    {
        float currentFrame = glfwGetTime();
        time.Update(currentFrame);

        Renderer::RendererCommand::EnableDepthTesting();
        Renderer::RendererCommand::ClearColor(m_Window->m_WindowColor);
        Renderer::RendererCommand::Clear();

        gridShader.Use();

        // Camera setup (will always revolve around grid even if it isn't drawn)
        pCamera->SetProjectionMatrix(gridShader.ID,
                                     pCamera,
                                     m_Window->GetWidth(),
                                     m_Window->GetHeight(),
                                     nearClip,
                                     farClip);

        pCamera->SetViewMatrix(gridShader.ID, pCamera);
        auto gridPos = glm::vec3(0, 0, 0);
        pCamera->SetModel(gridShader.ID, gridPos);

        if (m_DefaultGrid)
        {
            Renderer::RendererCommand::EnableBlending();
            Renderer::RendererCommand::DisableDepthMask();

            Renderer::RendererCommand::BindVertexArray(VAO[0]);
            Renderer::RendererCommand::SetUniformMat4(gridShader.ID, "model", pCamera->GetModel());
            Renderer::RendererCommand::SetUniformMat4(gridShader.ID, "view", pCamera->GetViewMatrix());
            Renderer::RendererCommand::SetUniformMat4(gridShader.ID, "projection", pCamera->GetProjectionMatrix());

            Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles,
                                                        0,
                                                        sizeof(gridVertices) / 3 * sizeof(float));

            Renderer::RendererCommand::DisableBlending();
            Renderer::RendererCommand::EnableDepthMask();
        }

        cubeShader.Use();
        Renderer::RendererCommand::BindVertexArray(VAO[1]);

        pCamera->SetModel(cubeShader.ID, glm::vec3(0.0f));
        float angle = 1.0f * currentFrame;
        pCamera->SetModelTransform(cubeShader.ID, glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));

        Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "model", pCamera->GetModel());
        Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "view", pCamera->GetViewMatrix());
        Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "projection", pCamera->GetProjectionMatrix());
        Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);

        RenderLayers();

        Window::UpdateWindow(m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    for (int i = 0; i < 16; ++i)
    {
        Renderer::RendererCommand::Cleanup(VAO[i], VBO[i], shaderProgram[i]);
    }
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

    auto debugLayer = new DebugLayer;
    PushLayer(debugLayer);

    auto entityLayer = new EntityLayer;
    entityLayer->SetWindow(m_Window->m_Window);
    PushLayer(entityLayer);

    auto controlsLayer = new ControlsLayer;
    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::Exit, [this]() { m_Window->TerminateWindow(); });
    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ApplyBGColor,
                                  [this, controlsLayer]()
                                  {
                                      auto color                = controlsLayer->GetBGColor();
                                      m_Window->m_WindowColor.r = color.x;
                                      m_Window->m_WindowColor.g = color.y;
                                      m_Window->m_WindowColor.b = color.z;
                                      m_Window->m_WindowColor.a = color.w;
                                  });
    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ToggleWireframe,
                                  [this]()
                                  {
                                      if (m_Window->m_PolygonMode == Renderer::RendererAPI::PolygonDataType::PolygonLine)
                                      {
                                          Renderer::RendererCommand::SetPolygonMode(
                                              Renderer::RendererAPI::PolygonDataType::PolygonFill);

                                          m_Window->m_PolygonMode = Renderer::RendererAPI::PolygonDataType::PolygonFill;
                                      }
                                      else
                                      {
                                          Renderer::RendererCommand::SetPolygonMode(
                                              Renderer::RendererAPI::PolygonDataType::PolygonLine);

                                          m_Window->m_PolygonMode = Renderer::RendererAPI::PolygonDataType::PolygonLine;
                                      }
                                  });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ApplyCameraSens,
                                  [this, controlsLayer]()
                                  { m_Window->SetCameraSens(controlsLayer->GetCamSensitivity()); });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ToggleGrid,
                                  [this]()
                                  {
                                      {
                                          m_DefaultGrid = !m_DefaultGrid;
                                      }
                                  });

    m_Layers.push_back(*controlsLayer);
    PushLayer(controlsLayer);
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

void Application::InitializeDefaultGrid(
    unsigned* shaderProgram, unsigned* VBO, unsigned* VAO, unsigned* EBO, unsigned* textures)
{
    Renderer::RendererCommand::InitVertexArray(VAO[0]);
    Renderer::RendererCommand::InitVertexBuffer(VBO[0], gridVertices, sizeof(gridVertices));
    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    3 * sizeof(float),
                                                    0);

    Renderer::RendererCommand::InitVertexArray(VAO[1]);
    Renderer::RendererCommand::InitVertexBuffer(VBO[0], vertices, sizeof(vertices));
    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    5 * sizeof(float),
                                                    0);
}

} // namespace Core
} // namespace Moonstone
