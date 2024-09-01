#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "Renderer/Include/RendererAPI.h"
#include <glad/glad.h>

namespace Moonstone
{

namespace Renderer
{

class OpenGLRendererAPI : public RendererAPI
{
    public:
        virtual void EnableDepthTesting() override;
        virtual void EnableFaceCulling() override;
        virtual void DisableFaceCulling() override;
        virtual void ClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        virtual void InitVertexShader(unsigned& vertexShader, const char* vertexShaderSrc) override;
        virtual void InitFragmentShader(unsigned& fragmentShader, const char* fragmentShaderSrc) override;
        virtual void InitShaderProgram(unsigned& shaderProgram,
                                       unsigned& vertexShader,
                                       unsigned& fragmentShader) override;
        virtual void InitVertexArray(unsigned& VAO) override;
        virtual void InitVertexBuffer(unsigned& VBO, float* vertices, size_t size) override;
        virtual void BindVertexBuffer(unsigned& VBO) override;
        virtual void BindVertexArray(unsigned& VAO) override;
        virtual void InitElementBuffer(unsigned& EBO, unsigned* indices, size_t size) override;
        virtual void InitVertexAttributes(int               index,
                                          int               size,
                                          NumericalDataType type,
                                          BooleanDataType   normalize,
                                          size_t            stride,
                                          size_t            offset) override;

        virtual void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO, size_t size) override;
        virtual void SubmitDrawArrays(DrawMode drawMode, int index, int count) override;

        virtual void SetPolygonMode(PolygonDataType polygonMode) override;
        virtual void SetViewport(int width, int height) override;

        virtual void Cleanup(unsigned& VAO, unsigned& VBO, unsigned& shaderProgram) override;

        virtual void UseProgram(unsigned& ID) override;

        virtual void SetUniformBool(const unsigned& ID, const std::string& name, bool value) override;
        virtual void SetUniformInt(const unsigned& ID, const std::string& name, bool value) override;
        virtual void SetUniformFloat(const unsigned& ID, const std::string& name, bool value) override;
        virtual void SetUniformMat4(const unsigned& ID, const std::string& name, glm::mat4 value) override;
        virtual void SetUniformVec3(const unsigned& ID, const std::string& name, glm::vec3 value) override;

        virtual void CreateTexture(unsigned& texture) override;
        virtual void SetTextureParameters(TextureTarget        target,
                                          TextureParameterName paramName,
                                          TextureParameter     param) override;

        virtual void UploadTexture(TextureTarget     target,
                                   int               mipmapLevel,
                                   TextureFormat     texFormat,
                                   int               x,
                                   int               y,
                                   TextureFormat     imageDataType,
                                   NumericalDataType dataType,
                                   unsigned char*    texData) override;

        virtual void BindTexture(Texture texture, TextureTarget target, unsigned textureObject) override;

        virtual void EnableBlending() override;
        virtual void DisableBlending() override;
        virtual void EnableDepthMask() override;
        virtual void DisableDepthMask() override;

    private:
        inline static GLuint ToOpenGLShaderType(NumericalDataType type)
        {
            switch (type)
            {
                case NumericalDataType::Float:
                    return GL_FLOAT;
                    break;
                case NumericalDataType::Int:
                    return GL_INT;
                    break;
                case NumericalDataType::UnsignedByte:
                    return GL_UNSIGNED_BYTE;
                    break;
                default:
                    return 0;
                    break;
            }
        }

        inline static GLuint ToOpenGLBooleanType(BooleanDataType type)
        {
            switch (type)
            {
                case BooleanDataType::False:
                    return GL_FALSE;
                    break;
                case BooleanDataType::True:
                    return GL_TRUE;
                    break;
                default:
                    return 0;
                    break;
            }
        }

        inline static GLuint ToOpenGLTextureTarget(TextureTarget target)
        {
            switch (target)
            {
                case TextureTarget::Texture1D:
                    return GL_TEXTURE_1D;
                    break;
                case TextureTarget::Texture2D:
                    return GL_TEXTURE_2D;
                    break;
                case TextureTarget::Texture3D:
                    return GL_TEXTURE_1D;
                    break;
                default:
                    return 0;
                    break;
            }
        }

        inline static GLuint ToOpenGLTextureParameterName(TextureParameterName paramName)
        {
            switch (paramName)
            {
                case TextureParameterName::TextureWrapS:
                    return GL_TEXTURE_WRAP_S;
                    break;
                case TextureParameterName::TextureWrapT:
                    return GL_TEXTURE_WRAP_T;
                    break;
                case TextureParameterName::TextureFilteringMin:
                    return GL_TEXTURE_MIN_FILTER;
                    break;
                case TextureParameterName::TextureFilteringMag:
                    return GL_TEXTURE_MAG_FILTER;
                    break;
                default:
                    return 0;
                    break;
            }
        }

        inline static GLuint ToOpenGLTextureParameter(TextureParameter param)
        {
            switch (param)
            {
                case TextureParameter::LinearMipmapLinear:
                    return GL_LINEAR_MIPMAP_LINEAR;
                case TextureParameter::Linear:
                    return GL_LINEAR;
                case TextureParameter::Repeat:
                    return GL_REPEAT;
                default:
                    return 0;
            }
        }

        inline static GLuint ToOpenGLTextureFormat(TextureFormat format)
        {
            switch (format)
            {
                case TextureFormat::Red:
                    return GL_RED;
                case TextureFormat::RGB:
                    return GL_RGB;
                case TextureFormat::RGBA:
                    return GL_RGBA;
                default:
                    return 0;
            }
        }

        inline static GLuint ToOpenGLTexture(Texture texture)
        {
            switch (texture)
            {
                case Texture::Texture0:
                    return GL_TEXTURE0;
                    break;
                case Texture::Texture1:
                    return GL_TEXTURE1;
                    break;
                case Texture::Texture2:
                    return GL_TEXTURE2;
                    break;
                default:
                    return 0;
                    break;
            }
        }

        inline static GLuint ToOpenGLDrawMode(DrawMode mode)
        {
            switch (mode)
            {
                case DrawMode::Lines:
                    return GL_LINES;
                    break;
                case DrawMode::Triangles:
                    return GL_TRIANGLES;
                    break;
                case DrawMode::Polygons:
                    return GL_POLYGON;
                    break;
                default:
                    return 0;
                    break;
            }
        }
};

} // namespace Renderer

} // namespace Moonstone
#endif // OPENGLRENDERERAPI_H
