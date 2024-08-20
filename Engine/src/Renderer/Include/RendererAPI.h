#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include "Core/Include/Core.h"
#include <glm/glm.hpp>

namespace Moonstone
{

namespace Renderer
{

class RendererAPI
{
    public:
        enum class API
        {
            None,
            OpenGL,
            Vulkan
        };

    public:
        inline static API GetAPI() { return s_API; }

        virtual void ClearColor(const glm::vec4 &color) = 0;
        virtual void Clear()                            = 0;

    private:
        static API s_API;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERAPI_H
