#ifndef renderingCOMMAND_H
#define renderingCOMMAND_H

#include "Rendering/Include/RenderingAPI.h"

namespace Moonstone
{

namespace Rendering
{

class RenderingCommand
{
    public:
        inline static void EnableDepthTesting() { s_RenderingAPI->EnableDepthTesting(); }
        inline static void EnableFaceCulling() { s_RenderingAPI->EnableFaceCulling(); }
        inline static void DisableFaceCulling() { s_RenderingAPI->DisableFaceCulling(); }
        inline static void ClearColor(const glm::vec4 &color) { s_RenderingAPI->ClearColor(color); }
        inline static void Clear() { s_RenderingAPI->Clear(); }

        inline static void InitVertexShader(unsigned &vertexShader, const char *vertexShaderSrc)
        {
            s_RenderingAPI->InitVertexShader(vertexShader, vertexShaderSrc);
        }

        inline static void InitFragmentShader(unsigned &fragmentShader, const char *fragmentShaderSrc)
        {
            s_RenderingAPI->InitFragmentShader(fragmentShader, fragmentShaderSrc);
        }

        inline static void InitShaderProgram(unsigned &shaderProgram, unsigned &vertexShader, unsigned &fragmentShader)
        {
            s_RenderingAPI->InitShaderProgram(shaderProgram, vertexShader, fragmentShader);
        }

        inline static void InitVertexArray(unsigned &VAO) { s_RenderingAPI->InitVertexArray(VAO); };

        inline static void InitVertexBuffer(unsigned &VBO, float *vertices, size_t size)
        {
            s_RenderingAPI->InitVertexBuffer(VBO, vertices, size);
        };

        inline static void BindVertexBuffer(unsigned &VBO) { s_RenderingAPI->BindVertexBuffer(VBO); }
        inline static void BindVertexArray(unsigned &VAO) { s_RenderingAPI->BindVertexArray(VAO); }

        inline static void InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size)
        {
            s_RenderingAPI->InitElementBuffer(EBO, indices, size);
        };

        inline static void InitVertexAttributes(int                            index,
                                                int                            size,
                                                RenderingAPI::NumericalDataType type,
                                                RenderingAPI::BooleanDataType   normalize,
                                                size_t                         stride,
                                                size_t                         offset)
        {
            s_RenderingAPI->InitVertexAttributes(index, size, type, normalize, stride, offset);
        };

        inline static void SetPolygonMode(RenderingAPI::PolygonDataType dataType)
        {
            s_RenderingAPI->SetPolygonMode(dataType);
        };

        inline static void SetViewport(int width, int height) { s_RenderingAPI->SetViewport(width, height); }

        inline static void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO, size_t size)
        {
            s_RenderingAPI->SubmitDrawCommands(shaderProgram, VAO, size);
        };

        inline static void SubmitDrawArrays(RenderingAPI::DrawMode drawMode, int index, int count)
        {
            s_RenderingAPI->SubmitDrawArrays(drawMode, index, count);
        };

        inline static void Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram)
        {
            s_RenderingAPI->Cleanup(VAO, VBO, shaderProgram);
        };

        inline static void UseProgram(unsigned &ID) { s_RenderingAPI->UseProgram(ID); }

        inline static void SetUniformBool(const unsigned &ID, const std::string &name, bool value)
        {
            s_RenderingAPI->SetUniformBool(ID, name, value);
        };

        inline static void SetUniformInt(const unsigned &ID, const std::string &name, bool value)
        {
            s_RenderingAPI->SetUniformInt(ID, name, value);
        };

        inline static void SetUniformFloat(const unsigned &ID, const std::string &name, float value)
        {
            s_RenderingAPI->SetUniformFloat(ID, name, value);
        };

        inline static void SetUniformMat4(const unsigned &ID, const std::string &name, glm::mat4 value)
        {
            s_RenderingAPI->SetUniformMat4(ID, name, value);
        };

        inline static void SetUniformVec3(const unsigned &ID, const std::string &name, glm::vec3 value)
        {
            s_RenderingAPI->SetUniformVec3(ID, name, value);
        };

        inline static void CreateTexture(unsigned &texture) { s_RenderingAPI->CreateTexture(texture); }

        inline static void SetTextureParameters(RenderingAPI::TextureTarget        target,
                                                RenderingAPI::TextureParameterName paramName,
                                                RenderingAPI::TextureParameter     param)
        {
            s_RenderingAPI->SetTextureParameters(target, paramName, param);
        };

        inline static void UploadTexture(RenderingAPI::TextureTarget     target,
                                         int                            mipmapLevel,
                                         RenderingAPI::TextureFormat     texFormat,
                                         int                            x,
                                         int                            y,
                                         RenderingAPI::TextureFormat     imageDataType,
                                         RenderingAPI::NumericalDataType dataType,
                                         unsigned char                 *texData)
        {
            s_RenderingAPI->UploadTexture(target, mipmapLevel, texFormat, x, y, imageDataType, dataType, texData);
        };

        inline static void BindTexture(RenderingAPI::Texture       texture,
                                       RenderingAPI::TextureTarget target,
                                       unsigned                   textureObject)
        {
            s_RenderingAPI->BindTexture(texture, target, textureObject);
        }

        inline static void EnableBlending() { s_RenderingAPI->EnableBlending(); }

        inline static void DisableBlending() { s_RenderingAPI->DisableBlending(); }

        inline static void EnableDepthMask() { s_RenderingAPI->EnableDepthMask(); };

        inline static void DisableDepthMask() { s_RenderingAPI->DisableDepthMask(); };

        inline static void BindFrameBuffer(unsigned &FBO) { s_RenderingAPI->BindFrameBuffer(FBO); }

        inline static void DrawFrameBuffer(unsigned &shaderID, unsigned &quadVAO, unsigned &FBOTexMap)
        {
            s_RenderingAPI->DrawFrameBuffer(shaderID, quadVAO, FBOTexMap);
        };

        inline static void InitFrameBuffer(int      &width,
                                           int      &height,
                                           unsigned &FBOTextureMap,
                                           unsigned &FBODepthTexture,
                                           unsigned &FBO,
                                           unsigned &ScreenQuadVAO,
                                           unsigned &ScreenQuadVBO)
        {
            s_RenderingAPI
                ->InitFrameBuffer(width, height, FBOTextureMap, FBODepthTexture, FBO, ScreenQuadVAO, ScreenQuadVBO);
        };

        inline static void RescaleFramebuffer(unsigned &texMap, int &width, int &height)
        {
            s_RenderingAPI->RescaleFramebuffer(texMap, width, height);
        };

    private:
        static std::unique_ptr<RenderingAPI> s_RenderingAPI;
};

} // namespace Rendering

} // namespace Moonstone

#endif // renderingCOMMAND_H
