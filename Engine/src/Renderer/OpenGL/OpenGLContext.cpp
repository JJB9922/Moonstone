#include "Include/OpenGLContext.h"

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Constructs an OpenGLContext instance.
 *
 * Initializes the context with the specified GLFW window and asserts that
 * the window is not null.
 *
 * @param window A pointer to the GLFW window to associate with the OpenGL context.
 */
OpenGLContext::OpenGLContext(GLFWwindow *window)
    : m_Window(window)
{
    MS_ASSERT(window, "window is null");
}

/**
 * @brief Initializes the OpenGL context.
 *
 * Sets the current context to the specified GLFW window and loads OpenGL
 * functions using GLAD. Logs OpenGL information such as vendor, renderer,
 * and version.
 *
 * @note This function asserts that the GLAD loader was successful.
 */
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
