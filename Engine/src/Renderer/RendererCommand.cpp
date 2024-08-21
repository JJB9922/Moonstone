#include "Include/RendererCommand.h"
#include "Renderer/OpenGL/Include/OpenGLRendererAPI.h"
#include "Renderer/Vulkan/Include/VulkanRendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Pointer to the RendererAPI instance.
 *
 * This static member points to an instance of RendererAPI, which is initialized
 * based on the preprocessor directives. The actual instance depends on whether
 * `MS_OPENGL` or `MS_VULKAN` is defined.
 *
 * @note This is only defined if either `MS_OPENGL` or `MS_VULKAN` is set.
 */
#ifdef MS_OPENGL
RendererAPI* RendererCommand::s_RendererAPI = new OpenGLRendererAPI;
#endif

#ifdef MS_VULKAN
RendererAPI* RendererCommand::s_RendererAPI = new VulkanRendererAPI;
#endif

} // namespace Renderer

} // namespace Moonstone
