#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include "Core/Include/Core.h"
#include <glm/glm.hpp>

namespace Moonstone
{

namespace Renderer
{

class RendererAPI
{
    public:
        enum class API
        {
            None,
            OpenGL,
            Vulkan
        };

        enum class DataType
        {
            None,
            PolygonLine,
            PolygonFill
        };

    public:
        inline static API GetAPI() { return s_API; }

        virtual void ClearColor(const glm::vec4 &color) = 0;
        virtual void Clear()                                                                                      = 0;
        virtual void InitVertexShader(unsigned &vertexShader, const char *vertexShaderSrc)                        = 0;
        virtual void InitFragmentShader(unsigned &fragmentShader, const char *fragmentShaderSrc)                  = 0;
        virtual void InitShaderProgram(unsigned &shaderProgram, unsigned &vertexShader, unsigned &fragmentShader) = 0;
        virtual void InitVertexArray(unsigned &VAO)                                                               = 0;
        virtual void InitVertexBuffer(unsigned &VBO, float *vertices, size_t size)                                = 0;
        virtual void InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size)                             = 0;
        virtual void InitVertexAttributes()                                                                       = 0;

        virtual void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO) = 0;

        virtual void SetPolygonMode(DataType dataType) = 0;

        virtual void Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram) = 0;

    private:
        static API s_API;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERAPI_H
