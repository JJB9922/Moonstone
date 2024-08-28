#ifndef GRAPHICSCONTEXTROUTER_H
#define GRAPHICSCONTEXTROUTER_H

#include "Renderer/Platform/OpenGL/Include/OpenGLContext.h"

#include "Renderer/Include/GraphicsContext.h"
#include "Renderer/Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

class GraphicsContextRouter
{
    public:
        static std::unique_ptr<GraphicsContext> GetContext(GLFWwindow* window)
        {
            auto API = RendererAPI::GetAPI();

            if (API == RendererAPI::API::OpenGL)
            {
                std::unique_ptr<GraphicsContext> context = std::make_unique<OpenGLContext>(window);
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
