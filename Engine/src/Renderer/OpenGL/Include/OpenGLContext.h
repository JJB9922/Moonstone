#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "Core/Include/Core.h"
#include "Renderer/Include/GraphicsContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace Moonstone
{

namespace Renderer
{

class OpenGLContext : public GraphicsContext
{
    public:
        OpenGLContext(GLFWwindow* window);

        virtual void Init() override;

    private:
        GLFWwindow* m_Window;
};

} // namespace Renderer

} // namespace Moonstone

#endif // OPENGLCONTEXT_H
