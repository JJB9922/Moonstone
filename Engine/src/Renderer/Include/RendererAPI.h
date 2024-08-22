#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include "Core/Include/Core.h"
#include <glm/glm.hpp>

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Abstract base class for Renderer API implementations.
 * 
 * Defines an interface for rendering operations, such as setting clear color, 
 * initializing shaders, and submitting draw commands. The class also supports 
 * different rendering APIs, such as OpenGL and Vulkan, which can be queried 
 * through the static method GetAPI().
 */
class RendererAPI
{
    public:
        enum class API
        {
            None,
            OpenGL,
            Vulkan
        };

        enum class PolygonDataType
        {
            None,
            PolygonLine,
            PolygonFill,
        };

        enum class NumericalDataType
        {
            Float,
            Int,
            UnsignedByte,
        };

        enum class BooleanDataType
        {
            True,
            False
        };

        enum class TextureTarget
        {
            Texture1D,
            Texture2D,
            Texture3D
        };

        enum class TextureParameterName
        {
            TextureWrapS,
            TextureWrapT,
            TextureFilteringMin,
            TextureFilteringMag
        };

        enum class TextureParameter
        {
            Linear,
            Wrap,
            Repeat,
        };

        enum class TextureFormat
        {
            RGB,
            RGBA
        };

        // Goes up to 16 most of the time
        enum class Texture
        {
            Texture0,
            Texture1,
            Texture2,
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
        virtual void InitVertexAttributes(
            int index, int size, NumericalDataType type, BooleanDataType normalize, size_t stride, size_t offset)
            = 0;

        virtual void SetPolygonMode(PolygonDataType dataType) = 0;

        virtual void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO) = 0;

        virtual void Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram) = 0;

        virtual void UseProgram(unsigned &ID) = 0;

        virtual void SetUniformBool(const unsigned &ID, const std::string &name, bool value)  = 0;
        virtual void SetUniformInt(const unsigned &ID, const std::string &name, bool value)   = 0;
        virtual void SetUniformFloat(const unsigned &ID, const std::string &name, bool value) = 0;

        virtual void CreateTexture(unsigned &texture) = 0;
        virtual void SetTextureParameters(TextureTarget target, TextureParameterName paramName, TextureParameter param)
            = 0;
        virtual void UploadTexture(TextureTarget     target,
                                   int               mipmapLevel,
                                   TextureFormat     texFormat,
                                   int               x,
                                   int               y,
                                   TextureFormat     imageDataType,
                                   NumericalDataType dataType,
                                   unsigned char    *texData)
            = 0;

        virtual void BindTexture(Texture texture, TextureTarget target, unsigned textureObject) = 0;

    private:
        /**
        * @brief Static member representing the current RendererAPI.
        *
        * This static member indicates the type of rendering API being used, set
        * based on the preprocessor directives. The value depends on whether
        * `MS_OPENGL` or `MS_VULKAN` is defined.
        *
        * @note This is only defined if either `MS_OPENGL` or `MS_VULKAN` is set.
        */
        static API s_API;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERAPI_H
