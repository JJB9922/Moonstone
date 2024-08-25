#include "Include/OpenGLRendererAPI.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Moonstone
{

namespace Renderer
{

void OpenGLRendererAPI::EnableDepthTesting() { glEnable(GL_DEPTH_TEST); }

void OpenGLRendererAPI::ClearColor(const glm::vec4 &color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::InitVertexShader(unsigned &vertexShader, const char *vertexShaderSrc)
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

void OpenGLRendererAPI::InitFragmentShader(unsigned &fragmentShader, const char *fragmentShaderSrc)
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

void OpenGLRendererAPI::InitShaderProgram(unsigned &shaderProgram, unsigned &vertexShader, unsigned &fragmentShader)
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

void OpenGLRendererAPI::InitVertexArray(unsigned &VAO)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
};

void OpenGLRendererAPI::InitVertexBuffer(unsigned &VBO, float *vertices, size_t size)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

void OpenGLRendererAPI::BindVertexBuffer(unsigned &VBO) { glBindBuffer(GL_ARRAY_BUFFER, VBO); }

void OpenGLRendererAPI::BindVertexArray(unsigned &VAO) { glBindVertexArray(VAO); }

void OpenGLRendererAPI::InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size)
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
};

void OpenGLRendererAPI::SetPolygonMode(PolygonDataType polygonMode)
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

void OpenGLRendererAPI::SetViewport(int width, int height) { glViewport(0, 0, width, height); }

void OpenGLRendererAPI::InitVertexAttributes(
    int index, int size, NumericalDataType type, BooleanDataType normalize, size_t stride, size_t offset)
{
    glVertexAttribPointer(index, size, ToOpenGLShaderType(type), ToOpenGLBooleanType(normalize), stride, (void *) offset);
    glEnableVertexAttribArray(index);
};

void OpenGLRendererAPI::SubmitDrawCommands(unsigned shaderProgram, unsigned VAO)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void OpenGLRendererAPI::SubmitDrawArrays(DrawMode drawMode, int index, int count)
{
    glDrawArrays(ToOpenGLDrawMode(drawMode), index, count);
};

void OpenGLRendererAPI::Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void OpenGLRendererAPI::UseProgram(unsigned &ID) { glUseProgram(ID); }

void OpenGLRendererAPI::SetUniformBool(const unsigned &ID, const std::string &name, bool value)
{
    glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), (int) value);
};

void OpenGLRendererAPI::SetUniformInt(const unsigned &ID, const std::string &name, bool value)
{
    glUniform1i(glad_glGetUniformLocation(ID, name.c_str()), value);
}

void OpenGLRendererAPI::SetUniformFloat(const unsigned &ID, const std::string &name, bool value)
{
    glUniform1f(glad_glGetUniformLocation(ID, name.c_str()), value);
}

void OpenGLRendererAPI::SetUniformMat4(const unsigned &ID, const std::string &name, glm::mat4 value)
{
    glad_glUniformMatrix4fv(glad_glGetUniformLocation(ID, name.c_str()),
                            1,
                            ToOpenGLBooleanType(RendererAPI::BooleanDataType::False),
                            glm::value_ptr(value));
}

void OpenGLRendererAPI::SetUniformVec3(const unsigned &ID, const std::string &name, glm::vec3 value)
{
    glad_glUniform3fv(glad_glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void OpenGLRendererAPI::CreateTexture(unsigned &texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void OpenGLRendererAPI::SetTextureParameters(TextureTarget        target,
                                             TextureParameterName paramName,
                                             TextureParameter     param)
{
    glTexParameteri(ToOpenGLTextureTarget(target),
                    ToOpenGLTextureParameterName(paramName),
                    ToOpenGLTextureParameter(param));
}

void OpenGLRendererAPI::UploadTexture(TextureTarget     target,
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

void OpenGLRendererAPI::BindTexture(Texture texture, TextureTarget target, unsigned textureObject)
{
    glActiveTexture(ToOpenGLTexture(texture));
    glBindTexture(ToOpenGLTextureTarget(target), textureObject);
}

void OpenGLRendererAPI::EnableBlending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLRendererAPI::DisableBlending() { glDisable(GL_BLEND); }

void OpenGLRendererAPI::EnableDepthMask() { glDepthMask(GL_TRUE); }

void OpenGLRendererAPI::DisableDepthMask() { glDepthMask(GL_FALSE); }

} // namespace Renderer

} // namespace Moonstone
