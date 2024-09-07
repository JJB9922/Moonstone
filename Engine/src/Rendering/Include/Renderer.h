#ifndef RENDERING_H
#define RENDERING_H

#include "Core/Include/Window.h"
#include "Include/EditorUI.h"
#include "Rendering/Include/Camera.h"
#include "Rendering/Include/RenderingCommand.h"
#include "Rendering/Include/Scene.h"
#include "Tools/Include/BaseShapes.h"

namespace Moonstone
{

namespace Rendering
{

class Renderer
{
  public:
    Renderer(std::shared_ptr<Scene> scene);

    inline void SetWindow(std::shared_ptr<Core::Window> window)
    {
        m_Window = window;
    }

    inline void SetSceneRenderTarget(std::shared_ptr<Core::EditorUI> editorUI)
    {
        m_SceneRenderTarget = editorUI;
    }

    void InitializeActiveCamera();
    void InitializeScene();
    void InitializeFramebuffer();

    void RenderScene();
    void SetupCamera();
    void RenderEditorGrid();
    void RenderVisibleObjects();
    void RenderLighting(SceneObject &object);

    void CleanupScene();
    void DeactivateDirectionalLight();
    void DeactivatePointLight(Lighting::Light &light);

  private:
    // Scene
    std::shared_ptr<Scene> m_Scene;
    std::vector<std::pair<std::string, Shader>> m_Shaders;
    std::shared_ptr<Core::Window> m_Window;

    // Objects
    unsigned m_VAO, m_VBO;

    // Frame Buffer
    std::shared_ptr<Core::EditorUI> m_SceneRenderTarget;
    unsigned m_FBShaderID, m_FBO, m_FBOTextureMap, m_FBODepthTexture, m_ScreenQuadVAO, m_ScreenQuadVBO;
};

} // namespace Rendering

} // namespace Moonstone

#endif // RENDERING_H
