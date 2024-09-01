#include "Include/Renderer.h"

namespace Moonstone
{

namespace Rendering
{

Renderer::Renderer() {}

void Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
    Rendering::RenderingCommand::EnableDepthTesting();
    Rendering::RenderingCommand::EnableFaceCulling();
    Rendering::RenderingCommand::ClearColor(scene->background);
    Rendering::RenderingCommand::Clear();
}

} // namespace Rendering

} // namespace Moonstone
