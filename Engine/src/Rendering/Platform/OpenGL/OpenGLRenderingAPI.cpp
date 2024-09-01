#include "Include/OpenGLRenderingAPI.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Moonstone
{

namespace Rendering
{

void OpenGLRenderingAPI::EnableDepthTesting() { glEnable(GL_DEPTH_TEST); }
void OpenGLRenderingAPI::EnableFaceCulling() { glEnable(GL_CULL_FACE); }
void OpenGLRenderingAPI::DisableFaceCulling() { glDisable(GL_CULL_FACE); }

void OpenGLRenderingAPI::ClearColor(const glm::vec4 &color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLRenderingAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRenderingAPI::InitVertexShader(unsigned &vertexShader, const char *vertexShaderSrc)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        MS_ERROR("vertex shader failed to compile: {0}", infoLog);
        return;
    }
}

void OpenGLRenderingAPI::InitFragmentShader(unsigned &fragmentShader, const char *fragmentShaderSrc)
{
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        MS_ERROR("fragment shader failed to compile: {0}", infoLog);
        return;
    }
}

void OpenGLRenderingAPI::InitShaderProgram(unsigned &shaderProgram, unsigned &vertexShader, unsigned &fragmentShader)
{
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        MS_ERROR("shader program failed to link: {0}", infoLog);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLRenderingAPI::InitVertexArray(unsigned &VAO)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
};

void OpenGLRenderingAPI::InitVertexBuffer(unsigned &VBO, float *vertices, size_t size)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

void OpenGLRenderingAPI::BindVertexBuffer(unsigned &VBO) { glBindBuffer(GL_ARRAY_BUFFER, VBO); }

void OpenGLRenderingAPI::BindVertexArray(unsigned &VAO) { glBindVertexArray(VAO); }

void OpenGLRenderingAPI::InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size)
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
};

void OpenGLRenderingAPI::SetPolygonMode(PolygonDataType polygonMode)
{
    switch (polygonMode)
    {
        case PolygonDataType::PolygonLine:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case PolygonDataType::PolygonFill:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        default:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
};

void OpenGLRenderingAPI::SetViewport(int width, int height) { glViewport(0, 0, width, height); }

void OpenGLRenderingAPI::InitVertexAttributes(
    int index, int size, NumericalDataType type, BooleanDataType normalize, size_t stride, size_t offset)
{
    glVertexAttribPointer(index, size, ToOpenGLShaderType(type), ToOpenGLBooleanType(normalize), stride, (void *) offset);
    glEnableVertexAttribArray(index);
};

void OpenGLRenderingAPI::SubmitDrawCommands(unsigned shaderProgram, unsigned VAO, size_t size)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void OpenGLRenderingAPI::SubmitDrawArrays(DrawMode drawMode, int index, int count)
{
    glDrawArrays(ToOpenGLDrawMode(drawMode), index, count);
};

void OpenGLRenderingAPI::Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void OpenGLRenderingAPI::UseProgram(unsigned &ID) { glUseProgram(ID); }

void OpenGLRenderingAPI::SetUniformBool(const unsigned &ID, const std::string &name, bool value)
{
    glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), (int) value);
};

void OpenGLRenderingAPI::SetUniformInt(const unsigned &ID, const std::string &name, bool value)
{
    glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), value);
}

void OpenGLRenderingAPI::SetUniformFloat(const unsigned &ID, const std::string &name, bool value)
{
    glUniform1f(glad_glGetUniformLocation(ID, name.c_str()), value);
}

void OpenGLRenderingAPI::SetUniformMat4(const unsigned &ID, const std::string &name, glm::mat4 value)
{
    glad_glUniformMatrix4fv(glad_glGetUniformLocation(ID, name.c_str()),
                            1,
                            ToOpenGLBooleanType(RenderingAPI::BooleanDataType::False),
                            glm::value_ptr(value));
}

void OpenGLRenderingAPI::SetUniformVec3(const unsigned &ID, const std::string &name, glm::vec3 value)
{
    glad_glUniform3fv(glad_glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLRenderingAPI::CreateTexture(unsigned &texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void OpenGLRenderingAPI::SetTextureParameters(TextureTarget        target,
                                              TextureParameterName paramName,
                                              TextureParameter     param)
{
    glTexParameteri(ToOpenGLTextureTarget(target),
                    ToOpenGLTextureParameterName(paramName),
                    ToOpenGLTextureParameter(param));
}

void OpenGLRenderingAPI::UploadTexture(TextureTarget     target,
                                       int               mipmapLevel,
                                       TextureFormat     texFormat,
                                       int               x,
                                       int               y,
                                       TextureFormat     imageDataType,
                                       NumericalDataType dataType,
                                       unsigned char    *texData)
{
    glTexImage2D(ToOpenGLTextureTarget(target),
                 mipmapLevel,
                 ToOpenGLTextureFormat(texFormat),
                 x,
                 y,
                 0,
                 ToOpenGLTextureFormat(imageDataType),
                 ToOpenGLShaderType(dataType),
                 texData);

    glGenerateMipmap(ToOpenGLTextureTarget(target));
}

void OpenGLRenderingAPI::BindTexture(Texture texture, TextureTarget target, unsigned textureObject)
{
    glActiveTexture(ToOpenGLTexture(texture));
    glBindTexture(ToOpenGLTextureTarget(target), textureObject);
}

void OpenGLRenderingAPI::EnableBlending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRenderingAPI::DisableBlending() { glDisable(GL_BLEND); }

void OpenGLRenderingAPI::EnableDepthMask() { glDepthMask(GL_TRUE); }

void OpenGLRenderingAPI::DisableDepthMask() { glDepthMask(GL_FALSE); }

void OpenGLRenderingAPI::BindFrameBuffer(unsigned &FBO) { glBindFramebuffer(GL_FRAMEBUFFER, FBO); }

void OpenGLRenderingAPI::DrawFrameBuffer(unsigned &shaderID, unsigned &quadVAO, unsigned &FBOTexMap)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    UseProgram(shaderID);
    glBindVertexArray(quadVAO);

    glBindTexture(GL_TEXTURE_2D, FBOTexMap);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderingAPI::InitFrameBuffer(int      &width,
                                         int      &height,
                                         unsigned &FBOTextureMap,
                                         unsigned &FBODepthTexture,
                                         unsigned &FBO,
                                         unsigned &ScreenQuadVAO,
                                         unsigned &ScreenQuadVBO)
{
    glViewport(0, 0, width, height);

    glGenTextures(1, &FBOTextureMap);
    glBindTexture(GL_TEXTURE_2D, FBOTextureMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &FBODepthTexture);
    glBindTexture(GL_TEXTURE_2D, FBODepthTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, (GLuint) FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOTextureMap, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, FBODepthTexture, 0);

    // End
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
        MS_DEBUG("framebuffer initialised successfully");
    else
        MS_ERROR("framebuffer did not initialise successfully");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenVertexArrays(1, &ScreenQuadVAO);
    glGenBuffers(1, &ScreenQuadVBO);
    glBindVertexArray(ScreenQuadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ScreenQuadVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 Tools::BaseShapes::screenQuadVerticesSize,
                 &Tools::BaseShapes::screenQuadVertices,
                 GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
}

void OpenGLRenderingAPI::RescaleFramebuffer(unsigned &texMap, int &width, int &height)
{
    glBindTexture(GL_TEXTURE_2D, texMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texMap, 0);
}

} // namespace Rendering

} // namespace Moonstone
