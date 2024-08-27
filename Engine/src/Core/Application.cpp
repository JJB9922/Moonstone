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
    m_Window  = std::shared_ptr<Window>(Window::CreateWindow());

    glm::vec3 cameraPos   = glm::vec3(0.0f, 4.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    float nearClip = 0.1f;
    float farClip  = 100.0f;

    auto pCamera = std::make_shared<Renderer::Camera>(cameraPos, cameraFront, cameraUp);

    m_Window->SetCamera(pCamera);

    std::string      gridVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.vert";
    std::string      gridFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.frag";
    Renderer::Shader gridShader(gridVert.c_str(), gridFrag.c_str());

    InitializeDefaultScene(m_ShaderProgram, m_VBO, m_VAO, m_EBO, m_Texture);
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

            Renderer::RendererCommand::BindVertexArray(m_VAO[0]);
            Renderer::RendererCommand::SetUniformMat4(gridShader.ID, "model", pCamera->GetModel());
            Renderer::RendererCommand::SetUniformMat4(gridShader.ID, "view", pCamera->GetViewMatrix());
            Renderer::RendererCommand::SetUniformMat4(gridShader.ID, "projection", pCamera->GetProjectionMatrix());

            Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles,
                                                        0,
                                                        Tools::BaseShapes::gridVerticesSize / 3 * sizeof(float));

            Renderer::RendererCommand::DisableBlending();
            Renderer::RendererCommand::EnableDepthMask();
        }

        for (int i = 0; i < m_Objects.size(); ++i)
        {
            // Model
            m_Objects[i].shader.Use();
            Renderer::RendererCommand::BindVertexArray(m_VAO[i + 1]);
            Renderer::RendererCommand::SetUniformMat4(m_Objects[i].shader.ID, "model", pCamera->GetModel());

            Renderer::RendererCommand::SetUniformMat4(m_Objects[i].shader.ID,
                                                      "model",
                                                      glm::translate(pCamera->GetModel(), m_Objects[i].position));

            Renderer::RendererCommand::SetUniformMat4(m_Objects[i].shader.ID, "view", pCamera->GetViewMatrix());
            Renderer::RendererCommand::SetUniformMat4(m_Objects[i].shader.ID,
                                                      "projection",
                                                      pCamera->GetProjectionMatrix());

            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "objectColor", {1.0f, 0.0f, 1.0f});

            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "material.diffuse", {0.6f, 0.6f, 0.6f});
            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "material.specular", {0.5f, 0.5f, 0.5f});
            Renderer::RendererCommand::SetUniformFloat(m_Objects[i].shader.ID, "material.shininess", 64.0f);

            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "viewPos", pCamera->GetPosition());

            // Directional Light
            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.direction", m_TimeOfDay);
            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.ambient", {0.3f, 0.3f, 0.3f});
            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.diffuse", {1.0f, 0.8f, 0.6f});
            Renderer::RendererCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.specular", {1.2f, 1.2f, 1.2f});
            Renderer::RendererCommand::SetUniformBool(m_Objects[i].shader.ID, "dirLight.isActive", m_SunLight);

            Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles,
                                                        0,
                                                        m_Objects[i].size);
        }

        RenderLayers();

        Window::UpdateWindow(m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    for (int i = 0; i < m_Objects.size(); ++i)
    {
        Renderer::RendererCommand::Cleanup(m_VAO[i], m_VBO[i], m_ShaderProgram[i]);
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
    entityLayer->SetBtnCallback(EntityLayer::ButtonID::ClearSelection,
                                [this, entityLayer]() { entityLayer->ClearEntitySelection(); });
    PushLayer(entityLayer);

    auto transformLayer = new TransformLayer;
    transformLayer->SetBtnCallbackObj(TransformLayer::ButtonID::RemoveObject,
                                      [this, entityLayer](Renderer::Scene::SceneObject& object)
                                      {
                                          auto it = std::find_if(m_Objects.begin(),
                                                                 m_Objects.end(),
                                                                 [&object](Renderer::Scene::SceneObject& obj)
                                                                 { return obj.name == object.name; });

                                          if (it != m_Objects.end())
                                          {
                                              m_Objects.erase(it);
                                          }

                                          entityLayer->ClearEntitySelection();
                                          entityLayer->RemoveObject(object);
                                      });

    transformLayer->SetSliderCallbackObj(TransformLayer::SliderID::PosGroup,
                                         [this](Renderer::Scene::SceneObject& object)
                                         {
                                             auto it = std::find_if(m_Objects.begin(),
                                                                    m_Objects.end(),
                                                                    [&object](Renderer::Scene::SceneObject& obj)
                                                                    { return obj.name == object.name; });

                                             if (it != m_Objects.end())
                                             {
                                                 it->position = object.position * 0.1f;
                                             }
                                         });
    PushLayer(transformLayer);

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

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::AddObject,
                                  [this, controlsLayer, entityLayer]()
                                  {
                                      ControlsLayer::SceneObject obj = controlsLayer->GetAddObject();

                                      switch (obj)
                                      {
                                          case ControlsLayer::SceneObject::Cube:
                                              AddCube(m_ShaderProgram, m_VBO, m_VAO, m_EBO, m_Texture);
                                              entityLayer->AddObject(m_Objects.back());
                                              entityLayer->ClearEntitySelection();
                                              break;
                                          case Moonstone::Core::ControlsLayer::SceneObject::Pyramid:
                                              AddPyramid(m_ShaderProgram, m_VBO, m_VAO, m_EBO, m_Texture);
                                              entityLayer->AddObject(m_Objects.back());
                                              entityLayer->ClearEntitySelection();
                                              break;
                                          default:
                                              break;
                                      }
                                  });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ToggleSunlight,
                                  [this, controlsLayer, entityLayer]()
                                  {
                                      ToggleSunlight();
                                  });

    controlsLayer->SetSliderCallback(ControlsLayer::SliderID::TimeOfDay,
                                     [this, controlsLayer](float timeOfDayFloat)
                                     {
                                         float timeOfDay = timeOfDayFloat;

                                         float angle = timeOfDay * 2.0f * 3.14159f;

                                         float y = cos(angle);
                                         float x = sin(angle);
                                         float z = 0.0f;

                                         glm::vec3 lightDirection = normalize(glm::vec3(x, y, z));

                                         m_TimeOfDay = lightDirection;
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

void Application::InitializeDefaultScene(std::vector<unsigned>& shaderProgram,
                                         std::vector<unsigned>& VBO,
                                         std::vector<unsigned>& VAO,
                                         std::vector<unsigned>& EBO,
                                         std::vector<unsigned>& textures)
{
    VAO.push_back(0);
    VBO.push_back(0);

    Renderer::RendererCommand::InitVertexArray(VAO.back());
    Renderer::RendererCommand::InitVertexBuffer(VBO.back(),
                                                Tools::BaseShapes::gridVertices,
                                                Tools::BaseShapes::gridVerticesSize);
    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    3 * sizeof(float),
                                                    0);
}

void Application::AddCube(std::vector<unsigned>& shaderProgram,
                          std::vector<unsigned>& VBO,
                          std::vector<unsigned>& VAO,
                          std::vector<unsigned>& EBO,
                          std::vector<unsigned>& textures)
{
    VAO.push_back(0);
    VBO.push_back(0);

    Renderer::RendererCommand::InitVertexArray(VAO.back());
    Renderer::RendererCommand::InitVertexBuffer(VBO.back(),
                                                Tools::BaseShapes::cubeVertices,
                                                Tools::BaseShapes::cubeVerticesSize);

    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    6 * sizeof(float),
                                                    0);

    Renderer::RendererCommand::InitVertexAttributes(1,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    6 * sizeof(float),
                                                    3 * sizeof(float));

    std::string      cubeVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.vert";
    std::string      cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.frag";
    Renderer::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    std::stringstream ss;
    ss << "default_cube_" << m_Objects.size();

    Renderer::Scene::SceneObject cube = {true, {0, 0, 0}, ss.str(), cubeShader, Tools::BaseShapes::cubeVerticesSize};

    m_Objects.push_back(cube);
}

void Application::AddPyramid(std::vector<unsigned>& shaderProgram,
                             std::vector<unsigned>& VBO,
                             std::vector<unsigned>& VAO,
                             std::vector<unsigned>& EBO,
                             std::vector<unsigned>& textures)
{
    VAO.push_back(0);
    VBO.push_back(0);

    Renderer::RendererCommand::InitVertexArray(VAO.back());
    Renderer::RendererCommand::InitVertexBuffer(VBO.back(),
                                                Tools::BaseShapes::pyramidVertices,
                                                Tools::BaseShapes::pyramidVerticesSize);
    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    5 * sizeof(float),
                                                    0);

    std::string      cubeVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.vert";
    std::string      cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.frag";
    Renderer::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    std::stringstream ss;
    ss << "default_pyramid_" << m_Objects.size();

    Renderer::Scene::SceneObject pyramid = {true, {0, 0, 0}, ss.str(), cubeShader, Tools::BaseShapes::cubeVerticesSize};

    m_Objects.push_back(pyramid);
}

} // namespace Core
} // namespace Moonstone
