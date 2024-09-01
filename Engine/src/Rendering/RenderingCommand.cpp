#include "Include/RenderingCommand.h"
#include "Rendering/Platform/OpenGL/Include/OpenGLRenderingAPI.h"
#include "Rendering/Platform/Vulkan/Include/VulkanRenderingAPI.h"

namespace Moonstone
{

namespace Rendering
{

#ifdef MS_OPENGL
std::unique_ptr<RenderingAPI> RenderingCommand::s_RenderingAPI = std::make_unique<OpenGLRenderingAPI>();
#endif

#ifdef MS_VULKAN
std::unique_ptr<RenderingAPI> RenderingCommand::s_RenderingAPI = std::make_unique<VulkanRenderingAPI>();
#endif

} // namespace Rendering

} // namespace Moonstone
