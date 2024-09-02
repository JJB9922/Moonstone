#ifndef RENDERING_H
#define RENDERING_H

#include "Core/Include/Window.h"
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

    void InitializeActiveCamera();
    void InitializeScene();
    void RenderScene();
    void RenderCamera();
    void RenderEditorGrid();
    void CleanupScene();

  private:
    std::shared_ptr<Scene> m_Scene;
    std::vector<std::pair<std::string, Shader>> m_Shaders;
    std::shared_ptr<Core::Window> m_Window;
    unsigned m_VAO, m_VBO;
};

} // namespace Rendering

} // namespace Moonstone

#endif // RENDERING_H
