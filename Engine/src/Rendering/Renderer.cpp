#include "Include/Renderer.h"

namespace Moonstone
{

namespace Rendering
{

Renderer::Renderer() {}

void Renderer::RenderScene(std::shared_ptr<Scene> scene) { MS_INFO("soemthing idk {0}", scene->sceneID); }

} // namespace Rendering

} // namespace Moonstone
