#ifndef GRAPHICSCONTEXTROUTER_H
#define GRAPHICSCONTEXTROUTER_H

#include "Rendering/Platform/OpenGL/Include/OpenGLContext.h"

#include "Rendering/Include/GraphicsContext.h"
#include "Rendering/Include/RenderingAPI.h"

namespace Moonstone
{

namespace Rendering
{

class GraphicsContextRouter
{
    public:
        static std::unique_ptr<GraphicsContext> GetContext(GLFWwindow* window)
        {
            auto API = RenderingAPI::GetAPI();

            if (API == RenderingAPI::API::OpenGL)
            {
                std::unique_ptr<GraphicsContext> context = std::make_unique<OpenGLContext>(window);
                return context;
            }
            else if (API == RenderingAPI::API::Vulkan)
            {
                MS_ERROR("vulkan not yet supported - switch to a different Rendering API");
                return nullptr;
            }

            return nullptr;
        }
};

} // namespace Rendering

} // namespace Moonstone

#endif // GRAPHICSCONTEXTROUTER_H
