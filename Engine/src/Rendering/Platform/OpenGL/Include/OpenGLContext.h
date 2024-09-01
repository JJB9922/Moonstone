#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "Core/Include/Core.h"
#include "Rendering/Include/GraphicsContext.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

namespace Moonstone
{

namespace Rendering
{

class OpenGLContext : public GraphicsContext
{
    public:
        OpenGLContext(GLFWwindow* window);

        virtual void Init();

    private:
        GLFWwindow* m_Window;
};

} // namespace Rendering

} // namespace Moonstone

#endif // OPENGLCONTEXT_H
