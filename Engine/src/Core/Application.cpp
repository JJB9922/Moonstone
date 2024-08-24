#include "Include/Application.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

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

    glm::vec3 cube = glm::vec3(0.0f, 0.0f, 0.0f);

    while (m_Running)
    {
        float currentFrame = glfwGetTime();
        time.SetDeltaTime(currentFrame - time.GetLastFrame());
        time.SetLastFrame(currentFrame);

        Renderer::RendererCommand::EnableDepthTesting();
        Renderer::RendererCommand::ClearColor(m_Window->m_WindowColor);
        Renderer::RendererCommand::Clear();

        Renderer::RendererCommand::BindTexture(Renderer::RendererAPI::Texture::Texture0,
                                               Renderer::RendererAPI::TextureTarget::Texture2D,
                                               textures[0]);
        Renderer::RendererCommand::BindTexture(Renderer::RendererAPI::Texture::Texture1,
                                               Renderer::RendererAPI::TextureTarget::Texture2D,
                                               textures[1]);

        cubeShader.Use();
        Renderer::RendererCommand::SetUniformVec3(cubeShader.ID, "objectColor", {1.0f, 0.5f, 0.3f});
        Renderer::RendererCommand::SetUniformVec3(cubeShader.ID, "lightColor", {1.0f, 1.0f, 1.0f});

        pCamera->SetProjectionMatrix(cubeShader.ID, pCamera, m_Window->GetWidth(), m_Window->GetHeight(), 0.1f, 100.0f);
        pCamera->SetViewMatrix(cubeShader.ID, pCamera);
        Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "projection", pCamera->GetProjectionMatrix());
        Renderer::RendererCommand::SetUniformMat4(cubeShader.ID, "view", pCamera->GetViewMatrix());

        pCamera->SetModel(cubeShader.ID, cubePos);
        Renderer::RendererCommand::BindVertexArray(VAO[0]);
        Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);

        lightShader.Use();
        Renderer::RendererCommand::SetUniformMat4(lightShader.ID, "projection", pCamera->GetProjectionMatrix());
        Renderer::RendererCommand::SetUniformMat4(lightShader.ID, "view", pCamera->GetViewMatrix());
        pCamera->SetModel(lightShader.ID, lightPos);
        pCamera->SetModelTransform(lightShader.ID, glm::scale(pCamera->GetModel(), glm::vec3(0.2f)));
        Renderer::RendererCommand::SetUniformMat4(lightShader.ID, "model", pCamera->GetModel());

        Renderer::RendererCommand::BindVertexArray(VAO[1]);
        Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);

        // render the cube
        //glBindVertexArray(cubeVAO);

        //Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);

        //Renderer::RendererCommand::SubmitDrawCommands(shaderProgram, VAO);

        RenderLayers();

        Window::UpdateWindow(m_Window->m_Window);

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
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f,
        0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f,
        0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,
        0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f,
    };

    Renderer::RendererCommand::InitVertexArray(VAO[0]);
    Renderer::RendererCommand::InitVertexBuffer(VBO[0], vertices, sizeof(vertices));
    //Renderer::RendererCommand::InitElementBuffer(EBO, indices, sizeof(indices));

    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    3 * sizeof(float),
                                                    0);

    Renderer::RendererCommand::InitVertexArray(VAO[1]);
    Renderer::RendererCommand::BindVertexBuffer(VBO[0]);
    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    3 * sizeof(float),
                                                    0);

    // -------------------------------------- Tex
    stbi_set_flip_vertically_on_load(true);
    std::string texturePath = std::string(RESOURCE_DIR) + "/Textures/container.jpg";

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
                                             Renderer::RendererAPI::TextureFormat::RGB,
                                             Renderer::RendererAPI::NumericalDataType::UnsignedByte,
                                             texData);

    stbi_image_free(texData);
}

} // namespace Core
} // namespace Moonstone
