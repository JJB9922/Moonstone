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

    unsigned shaderProgram[16], VBO[16], VAO[16], EBO[16];
    unsigned textures[16];

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    float nearClip = 0.1f;
    float farClip  = 100.0f;

    auto pCamera = std::make_shared<Renderer::Camera>(cameraPos, cameraFront, cameraUp);
    InitializeTestRenderData(shaderProgram, VBO, VAO, EBO, textures);

    std::string      vertexShaderPath   = std::string(RESOURCE_DIR) + "/Shaders/vshader.vs";
    std::string      fragmentShaderPath = std::string(RESOURCE_DIR) + "/Shaders/fshader.fs";

    std::string lightVertexShaderPath   = std::string(RESOURCE_DIR) + "/Shaders/lightvshader.vs";
    std::string lightFragmentShaderPath = std::string(RESOURCE_DIR) + "/Shaders/lightfshader.fs";

    Renderer::Shader cubeShader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
    Renderer::Shader lightShader(lightVertexShaderPath.c_str(), lightFragmentShaderPath.c_str());

    InitializeImGui();

    m_Window->SetCamera(pCamera);

    Time& time = Time::GetInstance();

    glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(2.0f, 5.0f, -15.0f),
                                 glm::vec3(-1.5f, -2.2f, -2.5f),
                                 glm::vec3(-3.8f, -2.0f, -12.3f),
                                 glm::vec3(2.4f, -0.4f, -3.5f),
                                 glm::vec3(-1.7f, 3.0f, -7.5f),
                                 glm::vec3(1.3f, -2.0f, -2.5f),
                                 glm::vec3(1.5f, 2.0f, -2.5f),
                                 glm::vec3(1.5f, 0.2f, -1.5f),
                                 glm::vec3(-1.3f, 1.0f, -1.5f)};

    glm::vec3 PointLightPosition[] = {glm::vec3(0.7f, 0.2f, 2.0f),
                                      glm::vec3(2.3f, -3.3f, -4.0f),
                                      glm::vec3(-4.0f, 2.0f, -12.0f),
                                      glm::vec3(0.0f, 0.0f, -3.0f)};

    glm::vec3 cube = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

    while (m_Running)
    {
        float currentFrame = glfwGetTime();
        time.SetDeltaTime(currentFrame - time.GetLastFrame());
        time.SetLastFrame(currentFrame);

        Renderer::RendererCommand::EnableDepthTesting();
        Renderer::RendererCommand::ClearColor(m_Window->m_WindowColor);
        Renderer::RendererCommand::Clear();

        cubeShader.Use();

        Renderer::RendererCommand::SetUniformInt(cubeShader.ID, "material.diffuse", 0);
        Renderer::RendererCommand::SetUniformInt(cubeShader.ID, "material.specular", 1);
        Renderer::RendererCommand::SetUniformFloat(cubeShader.ID, "material.shininess", 32.0f);

        Renderer::RendererCommand::SetUniformVec3(cubeShader.ID, "viewPos", pCamera->GetPosition());

        // Directional Light
        Renderer::RendererCommand::SetUniformVec3(cubeShader.ID, "dirLight.direction", {-0.2f, -0.1f, -0.3f});
        Renderer::RendererCommand::SetUniformVec3(cubeShader.ID, "dirLight.ambient", {0.1f, 0.1f, 0.1f});
        Renderer::RendererCommand::SetUniformVec3(cubeShader.ID, "dirLight.diffuse", {0.8f, 0.8f, 0.8f});
        Renderer::RendererCommand::SetUniformVec3(cubeShader.ID, "dirLight.specular", {1.0f, 1.0f, 1.0f});

        // Point Lights
        for (int i = 0; i < 4; i++)
        {
            Renderer::RendererCommand::SetUniformVec3(cubeShader.ID,
                                                      "pointLights[" + std::to_string(i) + "].position",
                                                      PointLightPosition[i]);
            Renderer::RendererCommand::SetUniformVec3(cubeShader.ID,
                                                      "pointLights[" + std::to_string(i) + "].ambient",
                                                      {0.1f, 0.1f, 0.1f});
            Renderer::RendererCommand::SetUniformVec3(cubeShader.ID,
                                                      "pointLights[" + std::to_string(i) + "].diffuse",
                                                      {0.8f, 0.8f, 0.8f});
            Renderer::RendererCommand::SetUniformVec3(cubeShader.ID,
                                                      "pointLights[" + std::to_string(i) + "].specular",
                                                      {1.0f, 1.0f, 1.0f});
            Renderer::RendererCommand::SetUniformFloat(cubeShader.ID,
                                                       "pointLights[" + std::to_string(i) + "].constant",
                                                       1.0f);
            Renderer::RendererCommand::SetUniformFloat(cubeShader.ID,
                                                       "pointLights[" + std::to_string(i) + "].linear",
                                                       0.09f);
            Renderer::RendererCommand::SetUniformFloat(cubeShader.ID,
                                                       "pointLights[" + std::to_string(i) + "].quadratic",
                                                       0.032f);
        }

        pCamera->SetProjectionMatrix(cubeShader.ID, pCamera, m_Window->GetWidth(), m_Window->GetHeight(), 0.1f, 100.0f);
        pCamera->SetViewMatrix(cubeShader.ID, pCamera);
        Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "projection", pCamera->GetProjectionMatrix());
        Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "view", pCamera->GetViewMatrix());

        Renderer::RendererCommand::BindTexture(Renderer::RendererAPI::Texture::Texture0,
                                               Renderer::RendererAPI::TextureTarget::Texture2D,
                                               textures[0]);

        Renderer::RendererCommand::BindTexture(Renderer::RendererAPI::Texture::Texture1,
                                               Renderer::RendererAPI::TextureTarget::Texture2D,
                                               textures[1]);

        Renderer::RendererCommand::BindVertexArray(VAO[0]);
        for (unsigned i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
            float     angle = 20.0f * i;
            model           = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "model", model);

            Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);
        }

        /*
        lightShader.Use();
        Renderer::RendererCommand::SetUniformMat4(lightShader.ID, "projection", pCamera->GetProjectionMatrix());
        Renderer::RendererCommand::SetUniformMat4(lightShader.ID, "view", pCamera->GetViewMatrix());

        pCamera->SetModel(lightShader.ID, lightPos);

        float radius       = 3.0f;
        float angularSpeed = 2.0f;

        float angle = angularSpeed * currentFrame;

        //lightPos.x = cubePos.x + radius * cos(angle);
        //lightPos.z = cubePos.z + radius * sin(angle);
        //lightPos.y = cubePos.y + radius * sin(angle) * cos(angle);

        pCamera->SetModelTransform(lightShader.ID, glm::scale(pCamera->GetModel(), glm::vec3(0.2f)));

        Renderer::RendererCommand::SetUniformMat4(lightShader.ID, "model", pCamera->GetModel());

        Renderer::RendererCommand::BindVertexArray(VAO[1]);
        Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);
*/
        // render the cube
        //glBindVertexArray(cubeVAO);

        //Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);

        //Renderer::RendererCommand::SubmitDrawCommands(shaderProgram, VAO);

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

void Application::InitializeTestRenderData(
    unsigned* shaderProgram, unsigned* VBO, unsigned* VAO, unsigned* EBO, unsigned* textures)
{
    float vertices[]
        = {-0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 0.0f,
           0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 1.0f, 1.0f,
           -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.0f, 0.0f,

           -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
           -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

           -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,  1.0f, 1.0f,
           -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
           -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  1.0f, 0.0f,

           0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
           0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
           0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

           -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  1.0f, 1.0f,
           0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  1.0f, 0.0f,
           -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f,

           -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
           0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
           -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.0f, 1.0f};

    Renderer::RendererCommand::InitVertexArray(VAO[0]);
    Renderer::RendererCommand::InitVertexBuffer(VBO[0], vertices, sizeof(vertices));
    //Renderer::RendererCommand::InitElementBuffer(EBO, indices, sizeof(indices));

    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    8 * sizeof(float),
                                                    0);

    Renderer::RendererCommand::InitVertexAttributes(1,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    8 * sizeof(float),
                                                    3 * sizeof(float));

    Renderer::RendererCommand::InitVertexAttributes(2,
                                                    2,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    8 * sizeof(float),
                                                    6 * sizeof(float));

    Renderer::RendererCommand::InitVertexArray(VAO[1]);
    Renderer::RendererCommand::BindVertexBuffer(VBO[0]);
    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    8 * sizeof(float),
                                                    0);

    // -------------------------------------- Tex
    stbi_set_flip_vertically_on_load(true);
    std::string texturePath = std::string(RESOURCE_DIR) + "/Textures/box.png";

    int            width, height, nrChannels;
    unsigned char* texData = Renderer::Textures::LoadTexture(texturePath.c_str(), width, height, nrChannels);

    Renderer::RendererCommand::CreateTexture(textures[0]);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureWrapS,
                                                    Renderer::RendererAPI::TextureParameter::Repeat);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureWrapT,
                                                    Renderer::RendererAPI::TextureParameter::Repeat);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureFilteringMin,
                                                    Renderer::RendererAPI::TextureParameter::Linear);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureFilteringMag,
                                                    Renderer::RendererAPI::TextureParameter::Linear);

    Renderer::RendererCommand::UploadTexture(Renderer::RendererAPI::TextureTarget::Texture2D,
                                             0,
                                             Renderer::RendererAPI::TextureFormat::RGB,
                                             width,
                                             height,
                                             Renderer::RendererAPI::TextureFormat::RGBA,
                                             Renderer::RendererAPI::NumericalDataType::UnsignedByte,
                                             texData);

    texturePath = std::string(RESOURCE_DIR) + "/Textures/box_specular.png";

    texData = Renderer::Textures::LoadTexture(texturePath.c_str(), width, height, nrChannels);

    Renderer::RendererCommand::CreateTexture(textures[1]);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureWrapS,
                                                    Renderer::RendererAPI::TextureParameter::Repeat);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureWrapT,
                                                    Renderer::RendererAPI::TextureParameter::Repeat);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureFilteringMin,
                                                    Renderer::RendererAPI::TextureParameter::Linear);

    Renderer::RendererCommand::SetTextureParameters(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                    Renderer::RendererAPI::TextureParameterName::TextureFilteringMag,
                                                    Renderer::RendererAPI::TextureParameter::Linear);

    Renderer::RendererCommand::UploadTexture(Renderer::RendererAPI::TextureTarget::Texture2D,
                                             0,
                                             Renderer::RendererAPI::TextureFormat::RGB,
                                             width,
                                             height,
                                             Renderer::RendererAPI::TextureFormat::RGBA,
                                             Renderer::RendererAPI::NumericalDataType::UnsignedByte,
                                             texData);

    stbi_image_free(texData);
}

} // namespace Core
} // namespace Moonstone
