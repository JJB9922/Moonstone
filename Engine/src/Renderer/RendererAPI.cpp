#include "Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Static member representing the current RendererAPI.
 *
 * This static member indicates the type of rendering API being used, set
 * based on the preprocessor directives. The value depends on whether
 * `MS_OPENGL` or `MS_VULKAN` is defined.
 *
 * @note This is only defined if either `MS_OPENGL` or `MS_VULKAN` is set.
 */
#ifdef MS_OPENGL
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif

#ifdef MS_VULKAN
RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
#endif

} // namespace Renderer

} // namespace Moonstone
