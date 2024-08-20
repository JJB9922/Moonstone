#include "Include/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Moonstone
{

namespace Renderer
{

void OpenGLRendererAPI::ClearColor(const glm::vec4 &color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT); }

} // namespace Renderer

} // namespace Moonstone
