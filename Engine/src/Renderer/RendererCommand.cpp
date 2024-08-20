#include "Include/RendererCommand.h"
#include "Renderer/OpenGL/Include/OpenGLRendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

#ifdef MS_OPENGL
RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
#endif

#ifdef MS_VULKAN
// ToDo: Vulkan API
#endif

} // namespace Renderer

} // namespace Moonstone
