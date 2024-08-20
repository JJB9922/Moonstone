#ifndef GRAPHICSCONTEXTROUTER_H
#define GRAPHICSCONTEXTROUTER_H

#include "Renderer/Include/GraphicsContext.h"
#include "Renderer/Include/RendererAPI.h"
#include "Renderer/OpenGL/Include/OpenGLContext.h"

namespace Moonstone
{

namespace Renderer
{

class GraphicsContextRouter
{
    public:
        static GraphicsContext* GetContext(GLFWwindow* window)
        {
            auto API = RendererAPI::GetAPI();

            if (API == RendererAPI::API::OpenGL)
            {
                GraphicsContext* context = new OpenGLContext(window);
                return context;
            }
            else if (API == RendererAPI::API::Vulkan)
            {
                MS_ERROR("vulkan not yet supported - switch to a different Renderer API");
                return nullptr;
            }

            return nullptr;
        }
};

} // namespace Renderer

} // namespace Moonstone

#endif // GRAPHICSCONTEXTROUTER_H
