#include "Include/RenderingAPI.h"

namespace Moonstone
{

namespace Rendering
{

#ifdef MS_OPENGL
RenderingAPI::API RenderingAPI::s_API = RenderingAPI::API::OpenGL;
#endif

#ifdef MS_VULKAN
RenderingAPI::API RenderingAPI::s_API = RenderingAPI::API::Vulkan;
#endif

} // namespace Rendering

} // namespace Moonstone
