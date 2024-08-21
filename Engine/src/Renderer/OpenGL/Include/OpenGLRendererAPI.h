#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "Renderer/Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

class OpenGLRendererAPI : public RendererAPI
{
    public:
        /**
 * @brief Sets the clear color for the OpenGL context.
 *
 * Configures the color that will be used when clearing the color buffer.
 *
 * @param color A glm::vec4 representing the RGBA color to set.
 */
        virtual void ClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void InitVertexShader(unsigned& vertexShader, const char* vertexShaderSrc) override;
        virtual void InitFragmentShader(unsigned& fragmentShader, const char* fragmentShaderSrc) override;
        virtual void InitShaderProgram(unsigned& shaderProgram,
                                       unsigned& vertexShader,
                                       unsigned& fragmentShader) override;
        virtual void InitVertexArray(unsigned& VAO) override;
        virtual void InitVertexBuffer(unsigned& VBO, float* vertices, size_t size) override;
        virtual void InitElementBuffer(unsigned& EBO, unsigned* indices, size_t size) override;
        virtual void InitVertexAttributes() override;

        virtual void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO) override;

        virtual void SetPolygonMode(DataType polygonMode) override;

        virtual void Cleanup(unsigned& VAO, unsigned& VBO, unsigned& shaderProgram) override;
};

} // namespace Renderer

} // namespace Moonstone
#endif // OPENGLRENDERERAPI_H
