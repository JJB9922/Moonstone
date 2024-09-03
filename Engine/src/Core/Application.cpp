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

} // namespace Core
//
} // namespace Moonstone
