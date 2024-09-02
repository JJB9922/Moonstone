#include "Include/Application.h"
#include "Include/EditorUI.h"
#include "mspch.h"
#include <memory>

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
    // UI
    std::shared_ptr<EditorUI> editorUI = std::make_shared<EditorUI>();
    m_EditorUI = editorUI;
    m_EditorUI->SetWindow(m_Window);

    // Scenes
    Rendering::SceneManager sceneManager;
    m_SceneManager = sceneManager;
    auto currentScene = m_SceneManager.LoadDefaultScene();
    m_SceneRenderer = sceneManager.InitializeSceneRenderer(currentScene);
    m_SceneRenderer->SetWindow(m_Window);
    m_SceneRenderer->InitializeActiveCamera();
    m_SceneRenderer->SetSceneRenderTarget(m_EditorUI);
    m_SceneRenderer->InitializeFramebuffer();

    // Init UI with Scene in place
    m_EditorUI->Init();
}

void Application::Run()
{
    m_Running = true;

    Time &time = Time::GetInstance();
    Rendering::Shader gridShader;
    Rendering::Shader meshShader;

    while (m_Running)
    {
        float currentFrame = glfwGetTime();
        time.Update(currentFrame);

        m_SceneRenderer->RenderScene();

        // UpdateCustomBaseShapes();

        m_EditorUI->Render();

        // Rendering::RenderingCommand::DrawFrameBuffer(m_FBShaderID, m_ScreenQuadVAO, m_FBOTexMap);
        Window::UpdateWindow(m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    m_SceneRenderer->CleanupScene();
}

void Application::UpdateCustomBaseShapes()
{
    for (int i = 0; i < m_Objects.size(); ++i)
    {
        // Model
        m_Objects[i].shader.Use();
        Rendering::RenderingCommand::BindVertexArray(m_VAO[i + 1]);
        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "model", m_ActiveCamera->GetModel());

        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "model",
                                                    glm::translate(m_ActiveCamera->GetModel(), m_Objects[i].position));

        glm::mat4 modelTransformMatrix =
            glm::translate(glm::mat4(1.0f), m_Objects[i].position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Objects[i].rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Objects[i].rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Objects[i].rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), m_Objects[i].scale);

        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "model", modelTransformMatrix);

        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "view", m_ActiveCamera->GetViewMatrix());
        Rendering::RenderingCommand::SetUniformMat4(m_Objects[i].shader.ID, "projection",
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

        Rendering::RenderingCommand::SubmitDrawArrays(Rendering::RenderingAPI::DrawMode::Triangles, 0,
                                                      m_Objects[i].size);
    }
}

std::unique_ptr<Application> CreateApplicationInstance()
{
    return std::make_unique<Application>(Application());
}

void Application::AddCube()
{
    m_VAO.push_back(0);
    m_VBO.push_back(0);

    Rendering::RenderingCommand::InitVertexArray(m_VAO.back());
    Rendering::RenderingCommand::InitVertexBuffer(m_VBO.back(), Tools::BaseShapes::cubeVertices,
                                                  Tools::BaseShapes::cubeVerticesSize);

    Rendering::RenderingCommand::InitVertexAttributes(0, 3, Rendering::RenderingAPI::NumericalDataType::Float,
                                                      Rendering::RenderingAPI::BooleanDataType::False,
                                                      6 * sizeof(float), 0);

    Rendering::RenderingCommand::InitVertexAttributes(1, 3, Rendering::RenderingAPI::NumericalDataType::Float,
                                                      Rendering::RenderingAPI::BooleanDataType::False,
                                                      6 * sizeof(float), 3 * sizeof(float));

    std::string cubeVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.vert";
    std::string cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.frag";
    Rendering::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    std::stringstream ss;
    ss << "default_cube_" << m_Objects.size();

    Rendering::SceneObject cube = {
        true, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, ss.str(), cubeShader, Tools::BaseShapes::cubeVerticesSize};
    m_Objects.push_back(cube);
}

} // namespace Core
} // namespace Moonstone
