#include "Include/OpenGLContext.h"

namespace Moonstone
{

namespace Renderer
{

OpenGLContext::OpenGLContext(GLFWwindow *window)
    : m_Window(window)
{
    MS_ASSERT(window, "window is null");
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_Window);
    int success = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    MS_ASSERT(success, "could not load glad");

    MS_INFO("OpenGL Info:");
    MS_INFO("  Vendor: {0}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
    MS_INFO("  Renderer: {0}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
    MS_INFO("  Version: {0}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}

} // namespace Renderer

} // namespace Moonstone
