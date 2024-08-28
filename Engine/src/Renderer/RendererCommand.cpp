#include "Include/RendererCommand.h"
#include "Renderer/Platform/OpenGL/Include/OpenGLRendererAPI.h"
#include "Renderer/Platform/Vulkan/Include/VulkanRendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

#ifdef MS_OPENGL
std::unique_ptr<RendererAPI> RendererCommand::s_RendererAPI = std::make_unique<OpenGLRendererAPI>();
#endif

#ifdef MS_VULKAN
std::unique_ptr<RendererAPI> RendererCommand::s_RendererAPI = std::make_unique<VulkanRendererAPI>();
#endif

} // namespace Renderer

} // namespace Moonstone
