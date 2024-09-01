#ifndef RENDERING_H
#define RENDERING_H

#include "Rendering/Include/Scene.h"
#include "mspch.h"

namespace Moonstone
{

namespace Rendering
{

class Renderer
{
    public:
        Renderer();

        void RenderScene(std::shared_ptr<Scene> scene);
        void RenderEditorGrid();
};

} // namespace Rendering

} // namespace Moonstone

#endif // RENDERING_H
