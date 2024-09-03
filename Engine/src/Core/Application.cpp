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

    while (m_Running)
    {
        float currentFrame = glfwGetTime();
        time.Update(currentFrame);

        m_SceneRenderer->RenderScene();

        m_EditorUI->Render();

        Window::UpdateWindow(m_Window);

        if (glfwWindowShouldClose(m_Window->m_Window))
            m_Running = false;
    }

    m_SceneRenderer->CleanupScene();
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
