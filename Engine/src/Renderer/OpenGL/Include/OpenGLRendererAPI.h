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

        virtual void SubmitDrawCommands(unsigned shaderProgram,
                                        unsigned VAO,
                                        unsigned texture,
                                        unsigned texture2) override;

        virtual void SetPolygonMode(DataType polygonMode) override;

        virtual void Cleanup(unsigned& VAO, unsigned& VBO, unsigned& shaderProgram) override;

        virtual void UseProgram(unsigned& ID) override;

        virtual void SetUniformBool(const unsigned& ID, const std::string& name, bool value) override;
        virtual void SetUniformInt(const unsigned& ID, const std::string& name, bool value) override;
        virtual void SetUniformFloat(const unsigned& ID, const std::string& name, bool value) override;

        virtual void CreateTexture(unsigned& texture) override;
};

} // namespace Renderer

} // namespace Moonstone
#endif // OPENGLRENDERERAPI_H
