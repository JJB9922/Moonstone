#ifndef RENDERINGAPI_H
#define RENDERINGAPI_H

#include "Core/Include/Core.h"
#include <glm/glm.hpp>

namespace Moonstone
{

namespace Rendering
{

class RenderingAPI
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

    enum class DrawMode
    {
        Lines,
        Triangles,
        Polygons
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
        LinearMipmapLinear,
        Wrap,
        Repeat,
    };

    enum class TextureFormat
    {
        Red,
        RGB,
        RGBA
    };

    enum Texture
    {
        Texture0 = 0,
        Texture1,
        Texture2,
        Texture3,
        Texture4,
        Texture5,
        Texture6,
        Texture7,
        Texture8,
        Texture9,
        Texture10,
        Texture11,
        Texture12,
        Texture13,
        Texture14,
        Texture15,
        Texture16,
    };

  public:
    inline static API GetAPI()
    {
        return s_API;
    }

    virtual void EnableDepthTesting() = 0;
    virtual void EnableFaceCulling() = 0;
    virtual void DisableFaceCulling() = 0;
    virtual void ClearColor(const glm::vec4 &color) = 0;
    virtual void Clear() = 0;
    virtual void InitVertexShader(unsigned &vertexShader, const char *vertexShaderSrc) = 0;
    virtual void InitFragmentShader(unsigned &fragmentShader, const char *fragmentShaderSrc) = 0;
    virtual void InitShaderProgram(unsigned &shaderProgram, unsigned &vertexShader, unsigned &fragmentShader) = 0;
    virtual void InitVertexArray(unsigned &VAO) = 0;
    virtual void InitVertexBuffer(unsigned &VBO, float *vertices, size_t size) = 0;
    virtual void BindVertexBuffer(unsigned &VBO) = 0;
    virtual void BindVertexArray(unsigned &VAO) = 0;
    virtual void InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size) = 0;
    virtual void InitVertexAttributes(int index, int size, NumericalDataType type, BooleanDataType normalize,
                                      size_t stride, size_t offset) = 0;

    virtual void SetPolygonMode(PolygonDataType dataType) = 0;
    virtual void SetViewport(int width, int height) = 0;

    virtual void SubmitDrawCommands(unsigned shaderProgram, unsigned VAO, size_t size) = 0;
    virtual void SubmitDrawArrays(DrawMode drawMode, int index, int count) = 0;

    virtual void Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram) = 0;

    virtual void UseProgram(unsigned &ID) = 0;

    virtual void SetUniformBool(const unsigned &ID, const std::string &name, bool value) = 0;
    virtual void SetUniformInt(const unsigned &ID, const std::string &name, bool value) = 0;
    virtual void SetUniformFloat(const unsigned &ID, const std::string &name, bool value) = 0;
    virtual void SetUniformMat4(const unsigned &ID, const std::string &name, glm::mat4 value) = 0;
    virtual void SetUniformVec3(const unsigned &ID, const std::string &name, glm::vec3 value) = 0;

    virtual void CreateTexture(unsigned &texture) = 0;
    virtual void SetTextureParameters(TextureTarget target, TextureParameterName paramName, TextureParameter param) = 0;
    virtual void UploadTexture(TextureTarget target, int mipmapLevel, TextureFormat texFormat, int x, int y,
                               TextureFormat imageDataType, NumericalDataType dataType, unsigned char *texData) = 0;

    virtual void BindTexture(Texture texture, TextureTarget target, unsigned textureObject) = 0;

    virtual void EnableBlending() = 0;
    virtual void DisableBlending() = 0;
    virtual void EnableDepthMask() = 0;
    virtual void DisableDepthMask() = 0;

    virtual void BindFrameBuffer(unsigned int &FBO) = 0;
    virtual void DrawFrameBuffer(unsigned &shaderID, unsigned &quadVAO, unsigned &FBOTexMap) = 0;
    virtual void InitFrameBuffer(int &width, int &height, unsigned &FBOTextureMap, unsigned &FBODepthTexture,
                                 unsigned &FBO, unsigned &ScreenQuadVAO, unsigned &ScreenQuadVBO) = 0;
    virtual void RescaleFramebuffer(unsigned &texMap, int &width, int &height) = 0;

  private:
    static API s_API;
};

} // namespace Rendering

} // namespace Moonstone

#endif // RENDERINGAPI_H
