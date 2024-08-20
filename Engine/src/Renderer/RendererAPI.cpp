#include "Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

#ifdef MS_OPENGL
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif

#ifdef MS_VULKAN
RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#endif
}

} // namespace Moonstone
