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
        RendererCommand();

    private:
        static RendererAPI *s_RendererAPI;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERCOMMAND_H
