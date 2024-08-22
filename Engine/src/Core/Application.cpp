#include "Include/Application.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

/**
 * @brief Static pointer to the current application instance.
 *
 * This is used to ensure only one instance of the Application class exists.
 */
Application* Application::s_ApplicationInstance = nullptr;

/**
 * @brief Constructs an Application instance.
 *
 * Ensures no other instance of Application exists and sets the static
 * instance pointer to this instance.
 */
Application::Application()
{
    if (s_ApplicationInstance)
    {
        MS_ASSERT(!s_ApplicationInstance, "application instance already exists");
    }

    s_ApplicationInstance = this;
}

/**
 * @brief Runs the application loop.
 *
 * Initializes the window and rendering data, then enters the main loop where
 * it clears the screen, submits draw commands, renders layers, and updates
 * the window until the window should close.
 */
void Application::Run()
{
    m_Running = true;
    m_Window  = std::unique_ptr<Window>(Window::CreateWindow());

    unsigned shaderProgram, VBO, VAO, EBO;
    unsigned textures[2];
    InitializeTestRenderData(shaderProgram, VBO, VAO, EBO, textures);

    std::string      vertexShaderPath   = std::string(RESOURCE_DIR) + "/Shaders/vshader.vs";
    std::string      fragmentShaderPath = std::string(RESOURCE_DIR) + "/Shaders/fshader.fs";

    Renderer::Shader shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    shader.Use();
    Renderer::RendererCommand::SetUniformInt(shader.ID, "texture", 0);
    Renderer::RendererCommand::SetUniformInt(shader.ID, "texture2", 1);

    InitializeImGui();

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    auto      pCamera     = std::make_shared<Renderer::Camera>(cameraPos, cameraFront, cameraUp);

    m_Window->SetCamera(pCamera);

    while (m_Running)
    {
        Renderer::RendererCommand::EnableDepthTesting();
        Renderer::RendererCommand::ClearColor(m_Window->m_WindowColor);
        Renderer::RendererCommand::Clear();

        Renderer::RendererCommand::BindTexture(Renderer::RendererAPI::Texture::Texture0,
                                               Renderer::RendererAPI::TextureTarget::Texture2D,
                                               textures[0]);
        Renderer::RendererCommand::BindTexture(Renderer::RendererAPI::Texture::Texture1,
                                               Renderer::RendererAPI::TextureTarget::Texture2D,
                                               textures[1]);

        shader.Use();

        glm::mat4 projection = glm::mat4(1.0);

        projection = glm::perspective(glm::radians(45.0f),
                                      (float) m_Window->GetWidth() / (float) m_Window->GetHeight(),
                                      0.1f,
                                      100.0f);

        glm::mat4 view;
        view = glm::lookAt(pCamera->GetPosition(), pCamera->GetPosition() + pCamera->GetFront(), pCamera->GetUp());

        Renderer::RendererCommand::SetUniformMat4(shader.ID, "view", view);
        Renderer::RendererCommand::SetUniformMat4(shader.ID, "projection", projection);

        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model           = glm::translate(model, cubePositions[i]);
            float angle     = 20.0f * i;
            model           = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            Renderer::RendererCommand::SetUniformMat4(shader.ID, "model", model);

            Renderer::RendererCommand::SubmitDrawArrays(Renderer::RendererAPI::DrawMode::Triangles, 0, 36);
        }

        //Renderer::RendererCommand::SubmitDrawCommands(shaderProgram, VAO);

        RenderLayers();

        Window::UpdateWindow(m_Window->m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    Renderer::RendererCommand::Cleanup(VAO, VBO, shaderProgram);
}

/**
 * @brief Renders all layers in the stack.
 *
 * Calls `OnUpdate` on each layer, then starts and ends the ImGui layer, and
 * calls `OnImGuiRender` on each layer.
 */
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

/**
 * @brief Initializes ImGui and adds it to the layer stack.
 *
 * Creates an ImGui layer, sets the window for ImGui, and sets up callback
 * functions for UI elements.
 */
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

/**
 * @brief Creates an instance of Application.
 *
 * Allocates and returns a new Application object.
 *
 * @return A pointer to the newly created Application instance.
 */
Application* CreateApplicationInstance() { return new Application(); }

/**
 * @brief Adds a layer to the layer stack.
 *
 * Pushes the layer onto the layer stack and calls `OnAttach` on it.
 *
 * @param layer A pointer to the Layer to add.
 */
void Application::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

/**
 * @brief Removes a layer from the layer stack.
 *
 * Removes the specified layer from the stack.
 *
 * @param layer A pointer to the Layer to remove.
 */
void Application::PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }

/**
 * @brief Adds an overlay layer to the layer stack.
 *
 * Pushes the overlay onto the layer stack and calls `OnAttach` on it.
 *
 * @param layer A pointer to the Layer to add as an overlay.
 */
void Application::PushOverlay(Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

/**
 * @brief Removes an overlay layer from the layer stack.
 *
 * Removes the specified overlay from the stack.
 *
 * @param overlay A pointer to the Layer to remove as an overlay.
 */
void Application::PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }

/**
 * @brief Initializes test rendering data.
 *
 * Sets up shaders, vertex arrays, buffers, and attributes for rendering.
 *
 * @param shaderProgram A reference to an unsigned integer to store the shader program ID.
 * @param VBO A reference to an unsigned integer to store the vertex buffer object ID.
 * @param VAO A reference to an unsigned integer to store the vertex array object ID.
 * @param EBO A reference to an unsigned integer to store the element buffer object ID.
 */
void Application::InitializeTestRenderData(
    unsigned& shaderProgram, unsigned& VBO, unsigned& VAO, unsigned& EBO, unsigned* textures)
{
    float vertices[]
        = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
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
    /*
    float vertices[] = {0.5f,  0.5f,  0.0f, 1.0f, 1.0f,

                        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,

                        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,

                        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};

    unsigned indices[] = {0,
                          1,
                          3,

                          1,
                          2,
                          3};

    float texCoords[] = {0.0f,
                         0.0f,

                         1.0f,
                         0.0f,

                         0.5f,
                         1.0f};
*/
    Renderer::RendererCommand::InitVertexArray(VAO);
    Renderer::RendererCommand::InitVertexBuffer(VBO, vertices, sizeof(vertices));
    //Renderer::RendererCommand::InitElementBuffer(EBO, indices, sizeof(indices));

    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    5 * sizeof(float),
                                                    0);
    Renderer::RendererCommand::InitVertexAttributes(1,
                                                    2,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    5 * sizeof(float),
                                                    3 * sizeof(float));

    stbi_set_flip_vertically_on_load(true);
    std::string texturePath  = std::string(RESOURCE_DIR) + "/Textures/rock.jpg";
    std::string texture2Path = std::string(RESOURCE_DIR) + "/Textures/geeble.png";

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

    unsigned char* texData2 = Renderer::Textures::LoadTexture(texture2Path.c_str(), width, height, nrChannels);

    Renderer::RendererCommand::UploadTexture(Renderer::RendererAPI::TextureTarget::Texture2D,
                                             0,
                                             Renderer::RendererAPI::TextureFormat::RGB,
                                             width,
                                             height,
                                             Renderer::RendererAPI::TextureFormat::RGBA,
                                             Renderer::RendererAPI::NumericalDataType::UnsignedByte,
                                             texData2);

    stbi_image_free(texData2);
}

} // namespace Core
} // namespace Moonstone
