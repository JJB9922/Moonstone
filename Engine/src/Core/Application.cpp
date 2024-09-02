#include "Include/Application.h"
#include "mspch.h"

namespace Moonstone
{
namespace Core
{

std::unique_ptr<Application> Application::s_ApplicationInstance = nullptr;

Application::Application()
{
    if (s_ApplicationInstance)
    {
        MS_ASSERT(!s_ApplicationInstance, "application instance already exists");
    }

    m_Window = std::shared_ptr<Window>(Window::CreateWindow());
    InitializeEditor();
}

void Application::InitializeEditor()
{
    // Scenes
    Rendering::SceneManager sceneManager;
    m_SceneManager = sceneManager;
    m_CurrentScene = m_SceneManager.LoadDefaultScene();

    // UI
    EditorUI editorUI;
    m_EditorUI = editorUI;
    m_EditorUI.SetWindow(m_Window);
    m_EditorUI.Init();
}

void Application::Run()
{
    m_Running = true;

    //  InitializeFramebuffer();
    //  InitializeCamera();

    // std::string      framebVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultfbo.vert";
    // std::string      framebFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultfbo.frag";
    // Rendering::Shader framebShader(framebVert.c_str(), framebFrag.c_str());
    // m_FBShaderID   = framebShader.ID;
    // unsigned empty = 0;

    Time&            time = Time::GetInstance();
    Rendering::Shader gridShader;
    Rendering::Shader meshShader;

    while (m_Running)
    {
        float currentFrame = glfwGetTime();
        time.Update(currentFrame);

        //Rendering::RenderingCommand::BindFrameBuffer(m_FBO);
        m_SceneManager.RenderScene(m_CurrentScene);

        //UpdateCamera();

        // if (m_DefaultGrid)
        // {
        //     UpdateGrid(gridShader);
        //}

        //UpdateCustomBaseShapes();

        //Rendering::RenderingCommand::DrawFrameBuffer(framebShader.ID, m_ScreenQuadVAO, m_FBOTextureMap);

        //  Rendering::RenderingCommand::BindFrameBuffer(empty);

        m_EditorUI.Render();

        Window::UpdateWindow(m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    for (int i = 0; i < m_Objects.size(); ++i)
    {
        Rendering::RenderingCommand::Cleanup(m_VAO[i], m_VBO[i], m_ShaderProgram[i]);
    }
}

void Application::InitializeCamera()
{
    glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    m_ActiveCamera = std::make_shared<Rendering::Camera>(cameraPos, cameraFront, cameraUp);

    m_Window->SetCamera(m_ActiveCamera);
}

void Application::UpdateCamera()
{
    float nearClip = 0.1f;
    float farClip  = 100.0f;

    m_ActiveCamera->SetProjectionMatrix(m_ActiveCamera, m_Window->GetWidth(), m_Window->GetHeight(), nearClip, farClip);
    m_ActiveCamera->SetViewMatrix(m_ActiveCamera);
    m_ActiveCamera->SetModel({0, 0, 0});
}

void Application::UpdateGrid(Rendering::Shader& gridShader)
{
    Rendering::RenderingCommand::DisableFaceCulling();

    if (!gridShader.ID)
    {
        std::string      gridVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.vert";
        std::string      gridFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.frag";
        Rendering::Shader loadedShader(gridVert.c_str(), gridFrag.c_str());
        gridShader = loadedShader;
        MS_ASSERT(gridShader.ID, "grid shader could not be set");
    }

    gridShader.Use();

    Rendering::RenderingCommand::EnableBlending();
    Rendering::RenderingCommand::DisableDepthMask();

    Rendering::RenderingCommand::BindVertexArray(m_VAO[0]);
    Rendering::RenderingCommand::SetUniformMat4(gridShader.ID, "model", m_ActiveCamera->GetModel());
    Rendering::RenderingCommand::SetUniformMat4(gridShader.ID, "view", m_ActiveCamera->GetViewMatrix());
    Rendering::RenderingCommand::SetUniformMat4(gridShader.ID, "projection", m_ActiveCamera->GetProjectionMatrix());

    Rendering::RenderingCommand::SubmitDrawArrays(Rendering::RenderingAPI::DrawMode::Triangles,
                                                  0,
                                                  Tools::BaseShapes::gridVerticesSize / 3 * sizeof(float));

    Rendering::RenderingCommand::DisableBlending();
    Rendering::RenderingCommand::EnableDepthMask();
    Rendering::RenderingCommand::EnableFaceCulling();
}

void Application::UpdateCustomBaseShapes()
{
    for (int i = 0; i < m_Objects.size(); ++i)
    {
        // Model
        m_Objects[i].shader.Use();
        Rendering::RenderingCommand::BindVertexArray(m_VAO[i + 1]);
        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "model", m_ActiveCamera->GetModel());

        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID,
                                                    "model",
                                                    glm::translate(m_ActiveCamera->GetModel(), m_Objects[i].position));

        glm::mat4 modelTransformMatrix
            = glm::translate(glm::mat4(1.0f), m_Objects[i].position)
              * glm::rotate(glm::mat4(1.0f), glm::radians(m_Objects[i].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
              * glm::rotate(glm::mat4(1.0f), glm::radians(m_Objects[i].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
              * glm::rotate(glm::mat4(1.0f), glm::radians(m_Objects[i].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
              * glm::scale(glm::mat4(1.0f), m_Objects[i].scale);

        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "model", modelTransformMatrix);

        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "view", m_ActiveCamera->GetViewMatrix());
        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID,
                                                    "projection",
                                                    m_ActiveCamera->GetProjectionMatrix());

        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "objectColor", {1.0f, 0.0f, 1.0f});

        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "material.diffuse", {0.6f, 0.6f, 0.6f});
        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "material.specular", {0.5f, 0.5f, 0.5f});
        Rendering::RenderingCommand::SetUniformFloat(m_Objects[i].shader.ID, "material.shininess", 64.0f);

        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "viewPos", m_ActiveCamera->GetPosition());

        // Directional Light
        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.direction", m_TimeOfDay);
        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.ambient", {0.3f, 0.3f, 0.3f});
        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.diffuse", {1.0f, 0.8f, 0.6f});
        Rendering::RenderingCommand::SetUniformVec3(m_Objects[i].shader.ID, "dirLight.specular", {1.2f, 1.2f, 1.2f});
        Rendering::RenderingCommand::SetUniformBool(m_Objects[i].shader.ID, "dirLight.isActive", m_SunLight);

        Rendering::RenderingCommand::SubmitDrawArrays(Rendering::RenderingAPI::DrawMode::Triangles,
                                                      0,
                                                      m_Objects[i].size);
    }
}

std::unique_ptr<Application> CreateApplicationInstance() { return std::make_unique<Application>(Application()); }

void Application::InitializeFramebuffer()
{
    int width, height;
    glfwGetWindowSize(m_Window->m_Window, &width, &height);
    Rendering::RenderingCommand::InitFrameBuffer(width,
                                                 height,
                                                 m_FBOTextureMap,
                                                 m_FBODepthTexture,
                                                 m_FBO,
                                                 m_ScreenQuadVAO,
                                                 m_ScreenQuadVBO);
}
/*
void Application::InitializeDefaultScene()
{
    m_VAO.push_back(0);
    m_VBO.push_back(0);

    Rendering::RenderingCommand::InitVertexArray(m_VAO.back());
    Rendering::RenderingCommand::InitVertexBuffer(m_VBO.back(),
                                                Tools::BaseShapes::gridVertices,
                                                Tools::BaseShapes::gridVerticesSize);
    Rendering::RenderingCommand::InitVertexAttributes(0,
                                                    3,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    3 * sizeof(float),
                                                    0);
}*/

void Application::AddCube()
{
    m_VAO.push_back(0);
    m_VBO.push_back(0);

    Rendering::RenderingCommand::InitVertexArray(m_VAO.back());
    Rendering::RenderingCommand::InitVertexBuffer(m_VBO.back(),
                                                  Tools::BaseShapes::cubeVertices,
                                                  Tools::BaseShapes::cubeVerticesSize);

    Rendering::RenderingCommand::InitVertexAttributes(0,
                                                      3,
                                                      Rendering::RenderingAPI::NumericalDataType::Float,
                                                      Rendering::RenderingAPI::BooleanDataType::False,
                                                      6 * sizeof(float),
                                                      0);

    Rendering::RenderingCommand::InitVertexAttributes(1,
                                                      3,
                                                      Rendering::RenderingAPI::NumericalDataType::Float,
                                                      Rendering::RenderingAPI::BooleanDataType::False,
                                                      6 * sizeof(float),
                                                      3 * sizeof(float));

    std::string      cubeVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.vert";
    std::string      cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.frag";
    Rendering::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    std::stringstream ss;
    ss << "default_cube_" << m_Objects.size();

    Rendering::SceneObject cube
        = {true, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, ss.str(), cubeShader, Tools::BaseShapes::cubeVerticesSize};
    m_Objects.push_back(cube);
}

} // namespace Core
} // namespace Moonstone
