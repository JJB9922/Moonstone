#ifndef RENDERERCOMMAND_H
#define RENDERERCOMMAND_H

#include "Renderer/Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

class RendererCommand
{
    public:
        inline static void ClearColor(const glm::vec4 &color) { s_RendererAPI->ClearColor(color); }

        inline static void Clear() { s_RendererAPI->Clear(); }

    private:
        static RendererAPI *s_RendererAPI;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERCOMMAND_H
