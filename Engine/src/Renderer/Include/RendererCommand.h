#ifndef RENDERERCOMMAND_H
#define RENDERERCOMMAND_H

#include "Renderer/Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Provides a static interface for renderer commands, forwarding calls to the underlying RendererAPI.
 */
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

        inline static void InitVertexAttributes(int                            index,
                                                int                            size,
                                                RendererAPI::NumericalDataType type,
                                                RendererAPI::BooleanDataType   normalize,
                                                size_t                         stride,
                                                size_t                         offset)
        {
            s_RendererAPI->InitVertexAttributes(index, size, type, normalize, stride, offset);
        };

        inline static void SetPolygonMode(RendererAPI::PolygonDataType dataType)
        {
            s_RendererAPI->SetPolygonMode(dataType);
        };

        inline static void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO)
        {
            s_RendererAPI->SubmitDrawCommands(shaderProgram, VAO);
        };

        inline static void Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram)
        {
            s_RendererAPI->Cleanup(VAO, VBO, shaderProgram);
        };

        inline static void UseProgram(unsigned &ID) { s_RendererAPI->UseProgram(ID); }

        inline static void SetUniformBool(const unsigned &ID, const std::string &name, bool value)
        {
            s_RendererAPI->SetUniformBool(ID, name, value);
        };

        inline static void SetUniformInt(const unsigned &ID, const std::string &name, bool value)
        {
            s_RendererAPI->SetUniformInt(ID, name, value);
        };

        inline static void SetUniformFloat(const unsigned &ID, const std::string &name, bool value)
        {
            s_RendererAPI->SetUniformFloat(ID, name, value);
        };

        inline static void CreateTexture(unsigned &texture) { s_RendererAPI->CreateTexture(texture); }
        inline static void SetTextureParameters(RendererAPI::TextureTarget        target,
                                                RendererAPI::TextureParameterName paramName,
                                                RendererAPI::TextureParameter     param)
        {
            s_RendererAPI->SetTextureParameters(target, paramName, param);
        };

        inline static void UploadTexture(RendererAPI::TextureTarget     target,
                                         int                            mipmapLevel,
                                         RendererAPI::TextureFormat     texFormat,
                                         int                            x,
                                         int                            y,
                                         RendererAPI::TextureFormat     imageDataType,
                                         RendererAPI::NumericalDataType dataType,
                                         unsigned char                 *texData)
        {
            s_RendererAPI->UploadTexture(target, mipmapLevel, texFormat, x, y, imageDataType, dataType, texData);
        };

        inline static void BindTexture(RendererAPI::Texture       texture,
                                       RendererAPI::TextureTarget target,
                                       unsigned                   textureObject)
        {
            s_RendererAPI->BindTexture(texture, target, textureObject);
        }

    private:
        /**
        * @brief Pointer to the RendererAPI instance.
        *
        * This static member points to an instance of RendererAPI, which is initialized
        * based on the preprocessor directives. The actual instance depends on whether
        * `MS_OPENGL` or `MS_VULKAN` is defined.
        *
        * @note This is only defined if either `MS_OPENGL` or `MS_VULKAN` is set.
        */
        static RendererAPI *s_RendererAPI;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERCOMMAND_H
