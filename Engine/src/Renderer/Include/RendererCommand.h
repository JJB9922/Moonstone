#ifndef RENDERERCOMMAND_H
#define RENDERERCOMMAND_H

#include "Renderer/Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

class RendererCommand
{
    public:
        inline static void ClearColor(const glm::vec4 &color) { s_RendererAPI->ClearColor(color); }
        inline static void Clear() { s_RendererAPI->Clear(); }

        inline static void InitVertexShader(unsigned &vertexShader, const char *vertexShaderSrc)
        {
            s_RendererAPI->InitVertexShader(vertexShader, vertexShaderSrc);
        }

        inline static void InitFragmentShader(unsigned &fragmentShader, const char *fragmentShaderSrc)
        {
            s_RendererAPI->InitFragmentShader(fragmentShader, fragmentShaderSrc);
        }

        inline static void InitShaderProgram(unsigned &shaderProgram, unsigned &vertexShader, unsigned &fragmentShader)
        {
            s_RendererAPI->InitShaderProgram(shaderProgram, vertexShader, fragmentShader);
        }

        inline static void InitVertexArray(unsigned &VAO) { s_RendererAPI->InitVertexArray(VAO); };

        inline static void InitVertexBuffer(unsigned &VBO, float *vertices, size_t size)
        {
            s_RendererAPI->InitVertexBuffer(VBO, vertices, size);
        };

        inline static void InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size)
        {
            s_RendererAPI->InitElementBuffer(EBO, indices, size);
        };

        inline static void InitVertexAttributes() { s_RendererAPI->InitVertexAttributes(); };

        inline static void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO)
        {
            s_RendererAPI->SubmitDrawCommands(shaderProgram, VAO);
        };

        inline static void SetPolygonMode(Renderer::RendererAPI::DataType polygonMode)
        {
            s_RendererAPI->SetPolygonMode(polygonMode);
        };

        inline static void Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram)
        {
            s_RendererAPI->Cleanup(VAO, VBO, shaderProgram);
        };

    private:
        static RendererAPI *s_RendererAPI;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERCOMMAND_H
