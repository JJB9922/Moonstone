#ifndef GRAPHICSCONTEXTROUTER_H
#define GRAPHICSCONTEXTROUTER_H

#include "Renderer/Include/GraphicsContext.h"
#include "Renderer/Include/RendererAPI.h"
#include "Renderer/OpenGL/Include/OpenGLContext.h"

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief A factory class that provides the appropriate graphics context based on the selected rendering API.
 * 
 * The `GraphicsContextRouter` class determines which graphics context implementation to use 
 * (e.g., OpenGL or Vulkan) by querying the current rendering API through `RendererAPI::GetAPI()`.
 * It is responsible for creating and returning an instance of the relevant graphics context.
 */
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
