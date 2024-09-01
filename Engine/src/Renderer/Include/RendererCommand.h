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
        inline static void EnableDepthTesting() { s_RendererAPI->EnableDepthTesting(); }
        inline static void EnableFaceCulling() { s_RendererAPI->EnableFaceCulling(); }
        inline static void DisableFaceCulling() { s_RendererAPI->DisableFaceCulling(); }
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

        inline static void BindVertexBuffer(unsigned &VBO) { s_RendererAPI->BindVertexBuffer(VBO); }
        inline static void BindVertexArray(unsigned &VAO) { s_RendererAPI->BindVertexArray(VAO); }

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

        inline static void SetViewport(int width, int height) { s_RendererAPI->SetViewport(width, height); }

        inline static void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO, size_t size)
        {
            s_RendererAPI->SubmitDrawCommands(shaderProgram, VAO, size);
        };

        inline static void SubmitDrawArrays(RendererAPI::DrawMode drawMode, int index, int count)
        {
            s_RendererAPI->SubmitDrawArrays(drawMode, index, count);
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

        inline static void SetUniformFloat(const unsigned &ID, const std::string &name, float value)
        {
            s_RendererAPI->SetUniformFloat(ID, name, value);
        };

        inline static void SetUniformMat4(const unsigned &ID, const std::string &name, glm::mat4 value)
        {
            s_RendererAPI->SetUniformMat4(ID, name, value);
        };

        inline static void SetUniformVec3(const unsigned &ID, const std::string &name, glm::vec3 value)
        {
            s_RendererAPI->SetUniformVec3(ID, name, value);
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

        inline static void EnableBlending() { s_RendererAPI->EnableBlending(); }

        inline static void DisableBlending() { s_RendererAPI->DisableBlending(); }

        inline static void EnableDepthMask() { s_RendererAPI->EnableDepthMask(); };

        inline static void DisableDepthMask() { s_RendererAPI->DisableDepthMask(); };

        inline static void BindFrameBuffer(unsigned &FBO) { s_RendererAPI->BindFrameBuffer(FBO); }

        inline static void DrawFrameBuffer(unsigned &shaderID, unsigned &quadVAO, unsigned &FBOTexMap)
        {
            s_RendererAPI->DrawFrameBuffer(shaderID, quadVAO, FBOTexMap);
        };

        inline static void InitFrameBuffer(int      &width,
                                           int      &height,
                                           unsigned &FBOTextureMap,
                                           unsigned &FBODepthTexture,
                                           unsigned &FBO,
                                           unsigned &ScreenQuadVAO,
                                           unsigned &ScreenQuadVBO)
        {
            s_RendererAPI
                ->InitFrameBuffer(width, height, FBOTextureMap, FBODepthTexture, FBO, ScreenQuadVAO, ScreenQuadVBO);
        };

        inline static void RescaleFramebuffer(unsigned &texMap, int &width, int &height)
        {
            s_RendererAPI->RescaleFramebuffer(texMap, width, height);
        };

    private:
        static std::unique_ptr<RendererAPI> s_RendererAPI;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERCOMMAND_H
