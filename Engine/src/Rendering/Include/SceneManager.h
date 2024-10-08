#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Rendering/Include/Camera.h"
#include "Rendering/Include/Lighting.h"
#include "Rendering/Include/Renderer.h"
#include "Rendering/Include/Scene.h"
#include "Rendering/Include/Shader.h"
#include "Tools/Include/BaseShapes.h"
#include "mspch.h"
// #include <glm/glm.hpp>

namespace Moonstone
{

namespace Rendering
{

class SceneManager
{
  public:
    SceneManager() = default;
    ~SceneManager() = default;

    // Scene
    std::shared_ptr<Scene> LoadDefaultScene();
    std::shared_ptr<Scene> LoadScene();

    std::unique_ptr<Renderer> InitializeSceneRenderer(std::shared_ptr<Scene> scene);

    void AddLightToScene(std::shared_ptr<Scene> scene, Lighting::Light &light);
    void AddObjectToScene(std::shared_ptr<Scene> scene);
    void AddModelToScene(std::shared_ptr<Scene> scene);

  private:
    // Grid
    void SetupEditorGrid();
    inline bool GetDefaultGridEnabled()
    {
        return m_IsEditorGridEnabled;
    }
    inline void SetDefaultGridEnabled(bool enabled)
    {
        m_IsEditorGridEnabled = enabled;
    }

  private:
    // Grid
    bool m_IsEditorGridEnabled;
};

} // namespace Rendering

} // namespace Moonstone

#endif // SCENEMANAGER_H
